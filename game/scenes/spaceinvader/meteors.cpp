#include "meteors.h"
#include "../../helper/inlinehelper.h"


Meteors::Meteors() : Node("Meteor"), m_spinSpeed(0), m_spinDirection(1), m_damage(0), m_speed(0),
                     m_collider(), m_animation(nullptr) {
}

void Meteors::Init() {
    Node::Init();
    m_globalTransformationFlag = Disable;
    m_globalTransform.SetSize(100,100);

    // setup collider
    m_collider = new ColliderNode(ft_CIRCLE);
    m_collider->OnCollision.Register<Meteors>(&Meteors::OnImpact, *this);
    AddChild(*m_collider);

    m_animation = new SpriteNode("../assets/Sprites/ball.png");
    //m_animation->m_spriteDisplayMode = Fit;
    AddChild(*m_animation);

    m_velocity = Vector2d{GetRandomPercentage(),GetRandomPercentage()};
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
    if (e->m_name == "Spaceship") {
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



