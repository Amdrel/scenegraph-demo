#pragma once

#include "nodes/node.hpp"

namespace scenegraphdemo {
    class PerspectiveCamera : public Node {
    public:
        // Projection matrix for the camera.
        glm::mat4 projectionMatrix;

        // View matrix for the camera.
        glm::mat4 viewMatrix;

        // View, and projection transforms combined.
        glm::mat4 viewProjectionMatrix;

        PerspectiveCamera() : PerspectiveCamera("PerspectiveCamera") {}
        PerspectiveCamera(std::string name) : PerspectiveCamera(name, VEC3_ZERO) {}
        PerspectiveCamera(std::string name, glm::vec3 position) :
            PerspectiveCamera(name, position, VEC3_ZERO) {}
        PerspectiveCamera(std::string name, glm::vec3 position, glm::vec3 rotation) :
            PerspectiveCamera(name, position, rotation, VEC3_ONE,
                glm::radians(45.0f), 1.0, 0.1, 100.0) {}
        PerspectiveCamera(
            std::string name,
            glm::vec3 position,
            glm::vec3 rotation,
            glm::vec3 scale,
            float fov,
            float aspect,
            float near,
            float far);

        // Traverses the tree and updates child transformations, and in addition
        // the view projection matrix as well.
        virtual void updateWorldTransform();
    private:
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 target = glm::vec3(0.0f, 0.0f, 1.0f);
    };
}
