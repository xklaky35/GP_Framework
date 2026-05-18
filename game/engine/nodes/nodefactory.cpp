#include "nodefactory.h"
#include "animatedspritenode.h"
#include "cameranode.h"
#include "collidernode.h"
#include "rigidbodynode.h"
#include "spritenode.h"
#include "../../helper/inlinehelper.h"
#include "ui/button.h"
#include "ui/hcontainer.h"
#include "ui/margincontainer.h"
#include "ui/panelcontainer.h"
#include "ui/textcontrol.h"
#include "ui/vcontainer.h"

namespace Engine {

    NodeFactory::NodeFactory() = default;
    NodeFactory::~NodeFactory() = default;


    NodeFactory* NodeFactory::s_pInstance = nullptr;

    NodeFactory & NodeFactory::GetInstance() {
        if (s_pInstance == nullptr) {
            s_pInstance = new NodeFactory();
        }
        return *s_pInstance;
    }

    void NodeFactory::DestroyInstance() {
        delete s_pInstance;
        s_pInstance = nullptr;
    }

    void NodeFactory::InitWithConfiguration(Node* n, const std::string& dataFilePath) {
        n->LoadConfigurationFile(dataFilePath);
        ConfigureIniNodesOf(n);
        ConfigureBaseNodesOf(n);
    }



    // custom child nodes setup
    void NodeFactory::ConfigureIniNodesOf(Node* n) {
        for (const auto& [childNodeId, childData] : n->GetChildConfiguration()) {
            // checks of current config is for custom or a base node

            // query node type
            auto nodeType = childData.find("nodeType");
            if (nodeType == childData.end()) {
                continue;
            }
            if (nodeType->second != NodeTypeStrings[NT_Custom]) {
                // if it is not a custom node confinue
                continue;
            }


            // query node type name
            auto nodeTypeName = childData.find("typeName");
            if (nodeType == childData.end()) {
                continue;
            }


            std::unique_ptr<Node> childToConfigure = Create(nodeTypeName->second);
            if (childToConfigure != nullptr) { // if the name is found in the registry

                // setup custom node with data stored in parents iniparser
                childToConfigure->SetupParameter(n->GetIniParser(), childNodeId);
                n->AddChild(*childToConfigure.release());
            }
        }
    }

    // base child nodes setup
    void NodeFactory::ConfigureBaseNodesOf(Node* n) {
        NodeConfiguration childConfiguration = n->GetChildConfiguration();
        for (const auto& [childNodeId, childData] : childConfiguration) {

            auto* nodeToConfigure = GetBaseNode(childData);

            if (nodeToConfigure != nullptr) {
                nodeToConfigure->SetupParameter(n->GetIniParser(), childNodeId);

                // this indicates it is a child node of a other node in this configuration file
                // this is not handled here (see CheckForNestedNodes())
                if (!nodeToConfigure->GetParentUId().empty()) {
                    continue;
                }

                n->AddChild(*nodeToConfigure);
                CheckForNestedNodes(*nodeToConfigure, childConfiguration, *n);
            }
        }
    }

    // this method checks the configuration file for any nested node on the currently added node
    void NodeFactory::CheckForNestedNodes(Node& newParentNode, NodeConfiguration& config, const Node& rootNode) {
        for (const auto& [childNodeId, childData] : config) {

            // get parent id
            auto parentId = childData.find("parentUId");
            if (parentId == childData.end()) {
                continue;
            }

            if (parentId->second != newParentNode.m_UId) {
                continue;
            }

            Node* nodeToConfigure = GetBaseNode(childData);

            if (nodeToConfigure != nullptr) {
                nodeToConfigure->SetupParameter(rootNode.GetIniParser(), childNodeId);
                newParentNode.AddChild(*nodeToConfigure);
                CheckForNestedNodes(*nodeToConfigure, config, rootNode);
            }
        }
    }

    Node* NodeFactory::GetBaseNode(const std::unordered_map<std::string, std::string>& childData) {

        // query node type
        auto nodeType = childData.find("nodeType");
        if (nodeType == childData.end()) {
            return nullptr;
        }

        int index = GetIndexOf(NodeTypeStrings, nodeType->second.c_str(), NODE_TYPE_COUNT);
        auto* newNode = CreateBaseNode(static_cast<NodeType>(index));
        return newNode;
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
        if (nodeType == NT_ColliderNode) {
            return dynamic_cast<Node*>(new ColliderNode());
        }
        if (nodeType == NT_TextControl) {
            return dynamic_cast<Node*>(new TextControl());
        }
        if (nodeType == NT_Button) {
            return dynamic_cast<Node*>(new Button());
        }
        if (nodeType == NT_HContainer) {
            return dynamic_cast<Node*>(new HContainer());
        }
        if (nodeType == NT_VContainer) {
            return dynamic_cast<Node*>(new VContainer());
        }
        if (nodeType == NT_PanelContainer) {
            return dynamic_cast<Node*>(new PanelContainer());
        }
        if (nodeType == NT_MarginContainer) {
            return dynamic_cast<Node*>(new MarginContainer());
        }
        return nullptr;
    }

    Node* NodeFactory::CreateCustomNode(const std::string& name, const std::string& path) {
        std::unique_ptr<Node> nodeToConfigure = Create(name);
        InitWithConfiguration(nodeToConfigure.get(), path);
        return nodeToConfigure.release();
    }
    void NodeFactory::RegisterClass(const std::string& name, Creator creator) {
        m_registry[name] = std::move(creator);
    }

    std::unique_ptr<Node> NodeFactory::Create(const std::string& name) {
        auto it = m_registry.find(name);
        if (it == m_registry.end()) return nullptr;
        return it->second();
    }

}

