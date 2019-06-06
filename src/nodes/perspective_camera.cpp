#include "logging.hpp"
#include "nodes/perspective_camera.hpp"
#include <glm/glm.hpp>
#include <iostream>

#define _USE_MATH_DEFINES

namespace scenegraphdemo {
    PerspectiveCamera::PerspectiveCamera(std::string name, glm::vec3 position,
            glm::vec3 rotation, glm::vec3 scale, float fov, float aspect,
            float near, float far) {
        this->name = name;
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;
        this->worldTransform = glm::mat4(1.0f);
        this->localTransform = glm::mat4(1.0f);
        this->projectionMatrix = glm::perspective(fov, aspect, near, far);
    }

    void PerspectiveCamera::updateWorldTransform() {
        if (dirty) {
            auto parent = this->parent.lock();
            this->updateLocalTransform();
            glm::mat4 parentWorldTransform;
            if (parent.get() != nullptr) {
                parentWorldTransform = parent.get()->worldTransform;
                worldTransform = parent.get()->worldTransform * localTransform;
            } else {
                worldTransform = glm::mat4() * localTransform;
            }

            // Get a normalized rotation vector (the forward vector) that points
            // in the direction the camera is facing that's 1 unit in length.
            // Also generate the up and right vectors from the forward vector.
            const auto decomposed = this->getDecomposedTransform();
            const auto eulerRotation = glm::eulerAngles(decomposed.rotation);
            forward = -glm::normalize(glm::vec3(
                -sin(eulerRotation.y),
                sin(eulerRotation.x) * cos(eulerRotation.y),
                cos(eulerRotation.x) * cos(eulerRotation.y)
            ));
            up = glm::cross(
                glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)),
                forward
            );
            right = glm::cross(forward, up);

            // Create new right and up vectors based on the roll.
            right = glm::normalize(
                right * cosf(rotation.z * M_PI) +
                up * sinf(rotation.z * M_PI)
            );
            up = glm::cross(forward, right);
            up.y = -up.y;

            // Adding the rotation results in a point 1 unit in front of the
            // camera which is then passed to the lookAt function to create a
            // view matrix. The projection matrix is pre-baked for performance.
            target = decomposed.translation + forward;
            viewMatrix = glm::lookAt(decomposed.translation, target, up);
            viewProjectionMatrix = projectionMatrix * viewMatrix;

            dirty = false;
            for (auto node : children) {
                node.get()->markDirty();
                node.get()->updateWorldTransform();
            }
        } else {
            for (auto node : children) {
                node.get()->updateWorldTransform();
            }
        }
    }
}
