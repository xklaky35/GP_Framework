#include "nodefactory.h"
#include "animatedspritenode.h"
#include "cameranode.h"
#include "rigidbodynode.h"
#include "spritenode.h"
#include "../../helper/inlinehelper.h"

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
                childToConfigure->SetupParameter(n->GetIniParser(), childNodeId);
                n->AddChild(*childToConfigure.release());
            }
        }
    }

    // base child nodes setup
    void NodeFactory::ConfigureBaseNodesOf(Node* n) {
        for (const auto& [childNodeId, childData] : n->GetChildConfiguration()) {

            std::string nodeType = n->GetTypeOfChildWithId(childNodeId);

            int index = GetIndexOf(NodeTypeStrings, nodeType.c_str(), NODE_TYPE_COUNT);

            Node* nodeToConfigure = CreateBaseNode(static_cast<NodeType>(index));


            if (nodeToConfigure != nullptr) {
                nodeToConfigure->SetupParameter(n->GetIniParser(), childNodeId);
                n->AddChild(*nodeToConfigure);
            }
        }
    }

    Node* NodeFactory::CreateBaseNode(NodeType nodeType) {

        // create child nodes
        if (nodeType == NT_Node) {
            return new Node();
        }
        if (nodeType == NT_SpriteNode) {
            return dynamic_cast<Node*>(new SpriteNode());
        }
        if (nodeType == NT_AnimatedSpriteNode) {
            return dynamic_cast<Node*>(new AnimatedSpriteNode());
        }
        if (nodeType == NT_RigidBodyNode) {
            return dynamic_cast<Node*>(new RigidbodyNode());
        }
        if (nodeType == NT_CameraNode) {
            return dynamic_cast<Node*>(new CameraNode());
        }
        return nullptr;
    }

    Node* NodeFactory::CreateCustomNode(const std::string& name, const std::string& path) {
        std::unique_ptr<Node> nodeToConfigure = Create(name);
        InitWithConfiguration(nodeToConfigure.get(), path);
        return nodeToConfigure.release();
    }

}

