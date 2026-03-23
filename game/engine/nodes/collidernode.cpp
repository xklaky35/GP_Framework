#include "collisionmanager.h"
#include "collidernode.h"

#include "forms/circle.h"
#include "forms/rectangle.h"

namespace Engine {
    ColliderNode::ColliderNode(Formtype type) : Node("Collider"), m_pForm(nullptr), m_type(type), m_pSpritenode(nullptr) {
        OnCollision = Event<Node>();
    }

    ColliderNode::~ColliderNode() {
        delete m_pForm;
        m_pForm = nullptr;

        Node::~Node();
    }


    void ColliderNode::Init() {
        Node::Init();

        CollisionManager::GetInstance().RegisterCollider(*this);

        // temporary debug indicator for collision area
        m_pSpritenode = new SpriteNode();
        m_pSpritenode->SetRGBA(1,0,0,0.3);
        m_pSpritenode->m_spriteDisplayMode = Fit;
        m_pSpritenode->m_globalTransform.SetSize(m_globalTransform.GetWidth(), m_globalTransform.GetHeight());

        switch (m_type) {
            case ft_CIRCLE: {
                m_pForm = new Circle(m_globalTransform);
                m_pSpritenode->SetSpritePath("../assets/Sprites/ball.png");
                break;
            }
            case ft_RECTANGLE: {
                m_pForm = new Rectangle(m_globalTransform);
                m_pSpritenode->SetSpritePath("../assets/Sprites/rect.png");
                break;
            }
        }
        AddChild(*m_pSpritenode);
    }


    //TODO: remove and switch to imgui debug
    void ColliderNode::UpdateForm() {
        m_pForm->m_transform = m_globalTransform;
    }


    void ColliderNode::SystemProcess() {
        Node::SystemProcess();
        UpdateForm();
    }

    void ColliderNode::Process(float deltaTime) {
        Node::Process(deltaTime);

        // keep form and sprite synced
        DetectCollition();
    }

    void ColliderNode::Draw(Renderer &renderer) {
        Node::Draw(renderer);
    }

    void ColliderNode::DrawDebug() {
        Node::DrawDebug();
    }

    const Form* ColliderNode::GetForm() const {
        return m_pForm;
    }

    void ColliderNode::DetectCollition() const {
        ColliderNode * col = CollisionManager::GetInstance().CheckForCollision(*this);
        if (col != nullptr) {
            OnCollision.Emit(col);
        }
    }
}
