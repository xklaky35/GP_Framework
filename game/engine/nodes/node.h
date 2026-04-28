#ifndef GP_FRAMEWORK_NODE_H
#define GP_FRAMEWORK_NODE_H

#include <functional>
#include <vector>

#include "../renderer.h"
#include "../structs/transform.h"
#include "../iniparser/iniparser.h"

namespace Engine {

    enum NodeType {
        NT_Node,
        NT_SpriteNode,
        NT_AnimatedSpriteNode,
        NT_RigidBody,
        NT_ColliderNode
    };
    static const char* NodeTypeStrings[] = {"Node", "SpriteNode", "AnimatedSpriteNode", "RigidBody", "ColliderNode"};



    enum InheritanceFlag {
        IF_Inherit,
        IF_Disable
    };
    static const char* InheritanceFlagStrings[] = { "Inherit", "Disable"};
    static constexpr int INHERITANCE_FLAG_STRINGS_COUNT = 2;


    struct NodeInfo;
    class Node {
    public:

        Node();

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
        std::string GetUId();

    public:
        std::string m_UId;
        std::vector<NodeInfo> m_nodeInfo;
        std::string m_dataFilePath;

        int m_Id;
        std::string m_name;
        bool m_bIsVisible;;
        Node* m_parent;

        // Flag indicating if this node inherits transformation information from the parent node
        InheritanceFlag m_globalTransformationFlag;

        NodeType m_nodeType;
        //Local transformation in the scope of the parent node
        Transform m_transform;
        //Global transformation using world coordinates
        Transform m_globalTransform;
        IniParser* m_iniParser;

    protected:
        std::vector<Node*> m_children;
        std::vector<Node*> m_childrenToAdd;
        std::vector<Node*> m_childrenToDelete;
    };

    struct NodeInfo
    {
        const char*     Name;       // Member name
        std::function<void(Node&)> Draw;
    };


}
#endif
