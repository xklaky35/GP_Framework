#include "node.h"
#include "../logmanager/logmanager.h"

namespace  Engine {
    Node::Node(const char* nodeName) : groupTag(nodeName), m_bIsRoot(false), parent(nullptr) {
        m_position = new Vector2d();
        m_transform = new Transform();
    }

    Node::~Node() {
        RemoveChildren();
        delete m_position;
        delete m_transform;
        m_transform = nullptr;
        m_position = nullptr;
    }

    void Node::Init() {}

    void Node::Process(const float deltaTime) {
        // bubbles up the node tree so that the root of the element dictates the global m_position
        if (parent != nullptr && parent->m_bIsRoot == false) {
            m_position->x = parent->m_position->x;
            m_position->y = parent->m_position->y;
        }

        m_position->x += m_transform->position->x;
        m_position->y += m_transform->position->y;

        for (Node* c : children) {
            c->Process(deltaTime);
        }
    }

    void Node::Draw(Renderer& renderer) {
        for (Node* c : children) {
            c->Draw(renderer);
        }
    }


    void Node::DrawDebug() {
        for (Node* c : children) {
            c->DrawDebug();
        }
    }

    Node* Node::GetChild(const char * nodeName) const {
        for (Node* n : children) {
            if (n->groupTag == nodeName) {
                return n;
            }
        }
        LogManager::GetInstance().Log(WARNING,"No child on \"%s\" with name \"%s\" found", &groupTag, nodeName);
        return nullptr;
    }

    void Node::AddChild(Node& node) {
        node.SetParent(this);
        children.push_back(&node);
        node.Init();
    }

    void Node::AddChildren(const std::vector<Node *>& nodes) {
        for (Node* c : nodes) {
            AddChild(*c);
        }
    }

    void Node::RemoveChild(Node* node) {
        std::erase<Node*>(children, node);
        delete node;
    }

    void Node::RemoveChildren() {
        for (const Node* c : children) {
            delete c;
        }
        children.clear();
    }

    void Node::SetParent(Node* node) {
        parent = node;
    }

    std::vector<Node*> Node::GetChildren() const {
        return children;
    }
}


