#ifndef GP_FRAMEWORK_NODE_H
#define GP_FRAMEWORK_NODE_H

#include <string>
#include <vector>

#include "../renderer.h"
#include "../structs/transform.h"
#include "../structs/vector2d.h"

namespace Engine {
    enum InheritanceFlag {
        Inherit,
        Disable
    };

    class Node {
    public:
        Node(const char * = "Node");
        virtual ~Node();
        virtual void Init();
        virtual void Process(float deltaTime);
        virtual void Draw(Renderer &);
        virtual void DrawDebug();
        const std::vector<Node *>& GetChildren() const;
        void AddChild(Node &);
        void AddChildren(const std::vector<Node *> &);
        void RemoveChild(Node *);
        void RemoveChildren();
        void SetParent(Node *);
        void ApplyLocalTransform();


    public:
        std::string m_name;

        // Flag indicating if this node inherits transformation information from the parent node
        InheritanceFlag m_globalTransformationFlag;

        //Local transformation in the scope of the parent node
        Transform m_transform;

        //Global transformation using world coordinates
        Transform m_globalTransform;

    protected:
        Node* parent;
        std::vector<Node*> children;
    };
}
#endif
