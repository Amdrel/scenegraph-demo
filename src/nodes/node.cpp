#include "logging.hpp"
#include "nodes/node.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>
#include <string>

namespace scenegraphdemo {
    Node::Node(std::string name, glm::vec3 position, glm::vec3 rotation,
            glm::vec3 scale) {
        this->name = name;
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;
        this->worldTransform = glm::mat4(1.0f);
        this->localTransform = glm::mat4(1.0f);
    }

    Node::~Node() {
        scenegraphdemo::debug("Removing Node \"" + name + "\"");
    }

    void Node::update(float delta) {
    }

    void Node::updateWorldTransform() {
        if (dirty) {
            auto parent = this->parent.lock();
            this->updateLocalTransform();
            if (parent.get() != nullptr) {
                worldTransform = parent.get()->worldTransform * localTransform;
            } else {
                worldTransform = glm::mat4(1.0f) * localTransform;
            }

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

    // TODO: Use quaternions as a substitute to euler angles to save on matrix
    // multiplications (and to learn quaternions).
    void Node::updateLocalTransform() {
        if (dirty) {
            auto transform = glm::mat4(1.0f);
            transform = glm::translate(transform, position);
            transform = glm::rotate(transform, rotation.y, glm::vec3(0.0, 1.0, 0.0));
            transform = glm::rotate(transform, rotation.x, glm::vec3(1.0, 0.0, 0.0));
            transform = glm::rotate(transform, rotation.z, glm::vec3(0.0, 0.0, 1.0));
            transform = glm::scale(transform, scale);
            localTransform = transform;
        }
    }

    void Node::add(std::shared_ptr<Node> node) {
        if (node) {
            node.get()->parent = shared_from_this();
            node.get()->dirty = true;
            children.push_back(node);
        }
    }

    void Node::remove() {
        auto parent = this->parent.lock();
        if (parent.get() == nullptr) {
            scenegraphdemo::warn("Attempted to remove \"" + name + "\", but it has no parent");
            return;
        }

        auto it = std::find_if(parent.get()->children.begin(), parent.get()->children.end(), [this](auto i) {
            return i.get() == this;
        });
        if (it != parent.get()->children.end()) {
            parent.get()->children.erase(it); // Order matters so no swap.
        }
    }

    void Node::markDirty() {
        dirty = true;
    }

    DecomposedTransform Node::getDecomposedTransform() {
        DecomposedTransform decomposed;
        glm::decompose(
            worldTransform,
            decomposed.scale,
            decomposed.rotation,
            decomposed.translation,
            decomposed.skew,
            decomposed.perspective
        );
        return decomposed;
    }

    glm::vec3 Node::getPos() const {
        return position;
    }

    glm::vec3 Node::getRot() const {
        return rotation;
    }

    glm::vec3 Node::getScale() const {
        return scale;
    }

    void Node::setPos(glm::vec3 position) {
        dirty = true;
        this->position = position;
    }

    void Node::setRot(glm::vec3 rotation) {
        dirty = true;
        this->rotation = rotation;
    }

    void Node::setScale(glm::vec3 scale) {
        dirty = true;
        this->scale = scale;
    }
}
