#include "nodefactory.h"
#include "animatedspritenode.h"
#include "spritenode.h"

namespace Engine {

    NodeFactory::NodeFactory() = default;
    NodeFactory::~NodeFactory() = default;


    NodeFactory* NodeFactory::m_pInstance = nullptr;

    NodeFactory & NodeFactory::GetInstance() {
        if (m_pInstance == nullptr) {
            m_pInstance = new NodeFactory();
        }
        return *m_pInstance;
    }

    void NodeFactory::DestroyInstance() {
        delete m_pInstance;
        m_pInstance = nullptr;
    }

    void NodeFactory::InitWithConfiguration(Node* n, const std::string& dataFilePath) {
        n->LoadConfigurationFile(dataFilePath);
        ConfigureIniNodesOf(n);
        ConfigureBaseNodesOf(n);
    }

    // custom child nodes setup
    void NodeFactory::ConfigureIniNodesOf(Node* n) {
        for (const auto& [childNodeId, childData] : n->GetChildConfiguration()) {

            if (!n->IsChildCustomNodeWithId(childNodeId)) {
                continue;
            }

            std::unique_ptr<Node> childToConfigure = Create(n->GetNameOfChildWithId(childNodeId));
            if (childToConfigure != nullptr) { // if the name is found in the registry

                // setup custom node with data stored in parents iniparser
                childToConfigure->Setup(n->GetIniParser(), childNodeId);
                n->AddChild(*childToConfigure.release());
            }
        }
    }

    // base child nodes setup
    void NodeFactory::ConfigureBaseNodesOf(Node* n) {
        for (const auto& [childNodeId, childData] : n->GetChildConfiguration()) {

            std::string nodeType = n->GetTypeOfChildWithId(childNodeId);
            Node* nodeToConfigure = nullptr;

            // create child nodes
            if (nodeType == NodeTypeStrings[NT_Node]) {
                nodeToConfigure = new Node();
                nodeToConfigure->Setup(n->GetIniParser(), childNodeId);
                n->AddChild(*nodeToConfigure);
            }
            if (nodeType == NodeTypeStrings[NT_SpriteNode]) {
                nodeToConfigure = new SpriteNode();
                nodeToConfigure->Setup(n->GetIniParser(), childNodeId);
                n->AddChild(*nodeToConfigure);
            }
            if (nodeType == NodeTypeStrings[NT_AnimatedSpriteNode]) {
                nodeToConfigure = new AnimatedSpriteNode();
                nodeToConfigure->Setup(n->GetIniParser(), childNodeId);
                n->AddChild(*nodeToConfigure);
            }
        }
    }

    Node* NodeFactory::CreateCustomNode(const std::string& name, const std::string& path) {
        std::unique_ptr<Node> nodeToConfigure = Create(name);
        InitWithConfiguration(nodeToConfigure.get(), path);
        return nodeToConfigure.release();
    }

}

