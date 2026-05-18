#ifndef GP_FRAMEWORK_NODEFACTORY_H
#define GP_FRAMEWORK_NODEFACTORY_H
#include <memory>

#include "node.h"


// Helper macro to register classes
// This macro takes the class name as input and saved it in the nodefactory instance
#define REGISTER_CLASS(ClassName) \
static bool _registered_##ClassName = []() { \
    NodeFactory::GetInstance().RegisterClass(#ClassName, []() { \
        return std::make_unique<ClassName>();\
    }); \
    return true; \
}();

namespace Engine {

    enum NodeType: int {
        NT_Node,
        NT_SpriteNode,
        NT_AnimatedSpriteNode,
        NT_RigidBodyNode,
        NT_ColliderNode,
        NT_CameraNode,
        NT_TextControl,
        NT_Button,
        NT_HContainer,
        NT_MarginContainer,
        NT_PanelContainer,
        NT_VContainer,
        NT_Control,
        NT_Custom
    };
    static const char* NodeTypeStrings[] = {
        "Node",
        "SpriteNode",
        "AnimatedSpriteNode",
        "RigidBody",
        "ColliderNode",
        "CameraNode",
        "TextControl",
        "Button",
        "HContainer",
        "MarginContainer",
        "PanelContainer",
        "VContainer",
        "Control",
        "Custom"
    };
    static constexpr int NODE_TYPE_COUNT = std::size(NodeTypeStrings);

    class NodeFactory {
    public:

        using Creator = std::function<std::unique_ptr<Node>()>;

        static NodeFactory &GetInstance();
        static void DestroyInstance();

        void InitWithConfiguration(Node*, const std::string&);
        void CheckForNestedNodes(Node& newParentNode, NodeConfiguration& config, const Node& rootNode);
        void ConfigureIniNodesOf(Node* n);
        void ConfigureBaseNodesOf(Node *n);

        Node *GetBaseNode(const std::unordered_map<std::string, std::string>& childData);
        Node *CreateBaseNode(NodeType nodeType);
        Node *CreateCustomNode(const std::string &name, const std::string &path);

        void RegisterClass(const std::string &name, Creator creator);

        std::unique_ptr<Node> Create(const std::string &name);

    private:
        NodeFactory();
        ~NodeFactory();


    private:
        std::unordered_map<std::string, Creator> m_registry;
        static NodeFactory *s_pInstance;

    };


}
#endif //GP_FRAMEWORK_NODEFACTORY_H