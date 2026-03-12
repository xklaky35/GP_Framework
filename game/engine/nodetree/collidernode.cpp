#include "collisionmanager.h"
#include "collidernode.h"

#include "forms/circle.h"
#include "forms/rectangle.h"

namespace Engine {
    ColliderNode::ColliderNode(Formtype type, const char *nodeName) : Node(nodeName), m_pOnEnter(nullptr), m_pOnExit(nullptr), m_pForm(nullptr), m_type(type), m_pSpritenode(nullptr) {}
    ColliderNode::~ColliderNode() = default;

    void ColliderNode::Init() {
        Node::Init();

        CollisionManager::GetInstance().RegisterCollider(*this);

        m_pSpritenode = new Spritenode();
        m_pSpritenode->SetRGBA(1,0,0,0.3);
        m_pSpritenode->m_bUseSpriteSize = false;

        switch (m_type) {
            case ft_CIRCLE: {
                m_pForm = new Circle(*m_position, m_transform->width/2);
                m_pSpritenode->SetSpritePath("../assets/Sprites/ball.png");
                break;
            }
            case ft_RECTANGLE: {
                m_pForm = new Rectangle(*m_position, m_transform->height, m_transform->width);
                m_pSpritenode->SetSpritePath("../assets/Sprites/rect.png");
                break;
            }
        }
        AddChild(*m_pSpritenode);
    }

    void ColliderNode::SyncFormAndSprite() {

        m_pForm->m_position.x = m_position->x + m_transform->position->x;
        m_pForm->m_position.y = m_position->y + m_transform->position->y;
        m_pForm->m_width = m_transform->width * m_transform->scale;
        m_pForm->m_height = m_transform->height * m_transform->scale;

        m_pSpritenode->m_transform->height = m_transform->height * m_transform->scale;
        m_pSpritenode->m_transform->width = m_transform->width * m_transform->scale;
    }

    void ColliderNode::Process(float deltaTime) {
        Node::Process(deltaTime);

        // keep form and sprite synced
        SyncFormAndSprite();
        DetectCollition();
    }

    void ColliderNode::Draw(Renderer &renderer) {
        Node::Draw(renderer);
    }

    const Form* ColliderNode::GetForm() const {
        return m_pForm;
    }

    void ColliderNode::DetectCollition() const {
        ColliderNode * col = CollisionManager::GetInstance().CheckForCollision(*this);
        if (col != nullptr && m_pOnEnter != nullptr) {
            m_pOnEnter(col);
        }
    }
}
