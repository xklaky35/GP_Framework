#include "node.h"
#include "../logmanager/logmanager.h"

namespace  Engine {
    Node::Node(const char* nodeName) : m_name(nodeName),
                                       m_globalTransformationFlag(Inherit),
                                       parent(nullptr) {
        m_globalTransform = Transform();
        m_transform = Transform();
    }

    Node::~Node() {
        RemoveChildren();
    }

    void Node::Init() {}

    void Node::Process(const float deltaTime) {
        // bubbles up the node tree so that the root of the element dictates the global m_position
        if (parent != nullptr && m_globalTransformationFlag == Inherit) {
            m_globalTransform = parent->m_globalTransform;
        }
        ApplyLocalTransform();

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

    const std::vector<Node*>& Node::GetChildren() const {
        return children;
    }

    void Node::ApplyLocalTransform() {
        // position
        m_globalTransform.position += m_transform.position;

        // angle
        const Vector2d vecX(cos(m_globalTransform.GetRotation()), sin(m_globalTransform.GetRotation()));
        const Vector2d vecY(-sin(m_globalTransform.GetRotation()), cos(m_globalTransform.GetRotation()));
        m_globalTransform.position = (vecX * m_globalTransform.position.x) + (vecY * m_globalTransform.position.y);

        // scale
        m_globalTransform.SetScale(m_globalTransform.GetScale() * m_transform.GetScale());

        // size
        m_globalTransform.SetHeight(m_globalTransform.GetHeight() + m_transform.GetHeight());
        m_globalTransform.SetWidth(m_globalTransform.GetWidth() + m_transform.GetWidth());
    }
}


