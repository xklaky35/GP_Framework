#ifndef GP_FRAMEWORK_NODE_H
#define GP_FRAMEWORK_NODE_H

#include <functional>
#include <string>
#include <vector>

#include "imgui.h"
#include "../renderer.h"
#include "../structs/transform.h"

namespace Engine {
    enum InheritanceFlag {
        Inherit,
        Disable
    };



    struct NodeInfo;
    class Node {
    public:

        Node(const char * = "Node");
        virtual ~Node();
        virtual void Init();
        virtual void Process(float deltaTime);
        virtual void SystemProcess();
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
        std::vector<NodeInfo> m_nodeInfo;

        int m_Id;
        const char* m_name;
        bool m_bIsVisible;;

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

    struct NodeInfo
    {
        const char*     Name;       // Member name
        std::function<void(Node&)> Draw;
    };


}
#endif
