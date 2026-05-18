#include "meteors.h"

#include <cstring>

#include "../../engine/logmanager/logmanager.h"
#include "../../helper/inlinehelper.h"


Meteors::Meteors() : m_spinSpeed(0), m_spinDirection(1), m_damage(0), m_speed(0), splitLevel(0),
                     maxLevel(0),
                     m_collider(), m_animation(nullptr) {
}

void Meteors::Init() {
    Node::Init();
    m_globalTransformationFlag = IF_Disable;
    m_globalTransform.SetSize(100,100);

    // setup collider
    m_collider = new ColliderNode();
    //m_collider->OnCollision.Register<Meteors>(&Meteors::OnImpact, *this);

    m_collider->m_transform.position.x += m_globalTransform.GetWidth() / 2;
    m_collider->m_transform.position.y += m_globalTransform.GetHeight() / 2;
    m_collider->m_transform.SetScale(m_globalTransform.GetScale());
    AddChild(*m_collider);

    m_animation = new SpriteNode();
    m_animation->SetSpritePath(std::string("../assets/Sprites/ball.png").data());
    //m_animation->m_spriteDisplayMode = Fit;
    AddChild(*m_animation);

    m_velocity = Vector2d{GetRandomPercentage() * GetRandomPosOrNeg(),GetRandomPercentage() * GetRandomPosOrNeg()};
    m_speed = GetRandomInt(1, 20);
}

void Meteors::Process(float deltaTime) {
    Node::Process(deltaTime);

    m_globalTransform.position += m_velocity * m_speed * deltaTime;
    Spin(deltaTime);
}


void Meteors::Spin(const float deltaTime) {
    m_transform.SetRotation(m_globalTransform.GetRotationDeg() + (m_spinSpeed * deltaTime) * static_cast<float>(m_spinDirection));
}

void Meteors::Draw(Renderer &renderer) {
    Node::Draw(renderer);
}

void Meteors::DrawDebug() {
    Node::DrawDebug();
}

void Meteors::OnImpact(const Node* e) {
    if (std::strcmp(e->m_name.c_str(), "Collider") != 0) {
        LogManager::GetInstance().Log(INFO, "Metheor hit %s", e->m_name.c_str());
        Split();
    }
}


void Meteors::SetSpinDirection(const signed char dir) {
    m_spinDirection = dir;
}

void Meteors::SetDamage(const int d) {
    m_damage = d;
}
void Meteors::SetSpinSpeed(const float ss) {
    m_spinSpeed = ss;
}

int Meteors::GetDamage() const {
    return m_damage;
}

void Meteors::Split() {

        Meteors* m1 = new Meteors();
        m1->m_globalTransform.position = m_globalTransform.position;
        m1->m_globalTransform.SetScale(m_globalTransform.GetScale() / 2);
        Meteors* m2 = new Meteors();
        m2->m_globalTransform.position = m_globalTransform.position;
        m2->m_globalTransform.SetScale(m_globalTransform.GetScale() / 2);
        Meteors* m3 = new Meteors();
        m3->m_globalTransform.position = m_globalTransform.position;
        m3->m_globalTransform.SetScale(m_globalTransform.GetScale() / 2);

        m_pParent->AddChild(*m1);
        m_pParent->AddChild(*m2);
        m_pParent->AddChild(*m3);
        m_pParent->RemoveChild(this);
}



