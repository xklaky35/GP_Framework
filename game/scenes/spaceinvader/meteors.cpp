#include "meteors.h"

#include "../../engine/nodes/animatedspritenode.h"

Meteors::Meteors() : Node("Meteor"), m_velocity(0), m_spinSpeed(0), m_spinDirection(1), m_damage(0),
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

    m_animation = new AnimatedSprite("../assets/Sprites/explosion.png", 64,64);
    m_animation->SetFrameDuration(0.3);
    m_animation->SetLooping(false);
    m_animation->StopAnimate();
    AddChild(*m_animation);
}

void Meteors::Process(float deltaTime) {
    Node::Process(deltaTime);

    m_globalTransform.position.y += m_velocity*deltaTime;
    Spin(deltaTime);
}


void Meteors::Spin(const float deltaTime) {
    m_transform.SetRotation(m_globalTransform.GetRotation() + (m_spinSpeed * deltaTime) * static_cast<float>(m_spinDirection));
}

void Meteors::Draw(Renderer &renderer) {
    Node::Draw(renderer);
}

void Meteors::DrawDebug() {
    Node::DrawDebug();
}

void Meteors::OnImpact(const Node* e) {
    if (e->m_name == "Spaceship") {
        m_animation->Animate();
    }
}

void Meteors::SetVelocity(const float v) {
    m_velocity = v;
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



