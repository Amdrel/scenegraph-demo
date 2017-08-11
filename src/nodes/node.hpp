#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include <memory>
#include <string>
#include <vector>

namespace scenegraphdemo {
    // Zeroed out 3D vector used in delegate constructors.
    static const glm::vec3 VEC3_ZERO(0, 0, 0);
    static const glm::vec3 VEC3_ONE(1, 1, 1);

    struct DecomposedTransform {
        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
    };

    class Node : public std::enable_shared_from_this<Node> {
    public:
        // Reference to the node's parent.
        std::weak_ptr<Node> parent;

        // Each node is and is part of an ordered tree that when traversed,
        // updates nodes at the top first and direct children in a first-come
        // first-serve basis.
        //
        // One thing to note is cyclic references are not forbidden, but they
        // should not stall the application as traversed nodes have a dirty
        // check. Referencing the same node in different trees can lead to odd
        // unexpected behavior in regards to coordinates and should be avoided.
        std::vector<std::shared_ptr<Node>> children;

        // World-space transformation for this node. This matrix is typically
        // used when generating the world matrix of children nodes.
        glm::mat4 worldTransform;

        // Local-space transformation for this node.
        glm::mat4 localTransform;

        // Human-readable name used to describe the node's function.
        std::string name;

        Node() : Node("Node") {}
        Node(std::string name) : Node(name, VEC3_ZERO) {}
        Node(std::string name, glm::vec3 position) :
            Node(name, position, VEC3_ZERO) {}
        Node(std::string name, glm::vec3 position, glm::vec3 rotation) :
            Node(name, position, rotation, VEC3_ONE) {}
        Node(
            std::string name,
            glm::vec3 position,
            glm::vec3 rotation,
            glm::vec3 scale);

        virtual ~Node();

        // Adds a another node as a direct child. The node will assume ownership
        // of the node added as a child.
        void add(std::shared_ptr<Node> node);

        // Removes the node from it's parent tree.
        void remove();

        // Traverses the tree and updates child transformations.
        virtual void updateWorldTransform();

        // Recreates the local-space transform based on pos, rot, and scale.
        virtual void updateLocalTransform();

        DecomposedTransform getDecomposedTransform();

        // Returns the node's position in local-space.
        glm::vec3 getPos() const;

        // Returns the node's rotation in local-space.
        glm::vec3 getRot() const;

        // Returns the node's scale in local-space.
        glm::vec3 getScale() const;

        // Sets the node's position in local-space.
        void setPos(glm::vec3 position);

        // Sets the node's rotation in local-space.
        void setRot(glm::vec3 rotation);

        // Sets the node's scale in local-space.
        void setScale(glm::vec3 scale);

        // Sets the diry flag for the node so transforms are updated later.
        void markDirty();

        // Runs the node's update function which can vary due to inheritance.
        virtual void update(float delta);
    protected:
        // Position of the node in local-space.
        glm::vec3 position;

        // Rotation of the node in local-space.
        glm::vec3 rotation;

        // Scale of the node in local-space.
        glm::vec3 scale;

        // Dirty flag used to speed up tree traversal and prevent cyclic loops.
        // This flag will be set when the transform is changed.
        bool dirty = true;
    };
}
