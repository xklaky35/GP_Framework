#include "collisionmanager.h"

#include "collidernode.h"

namespace Engine {
    ColliderNode::ColliderNode(Form *form, const char *nodeName) : Node(nodeName), m_pOnEnter(nullptr), m_pOnExit(nullptr), m_pForm(form) {}
    ColliderNode::~ColliderNode() = default;

    void ColliderNode::Init() {
        Node::Init();
        CollisionManager::GetInstance().RegisterCollider(*this);
    }

    void ColliderNode::Process(float deltaTime) {
        Node::Process(deltaTime);
        m_pForm->m_position = m_position;
        DetectCollition();
    }

    void ColliderNode::Draw(Renderer &renderer) {
        Node::Draw(renderer);
    }

    void ColliderNode::DetectCollition() const {
        ColliderNode * col = CollisionManager::GetInstance().CheckForCollision(*this);
        if (col != nullptr && m_pOnEnter != nullptr) {
            m_pOnEnter(col);
        }
    }
}
