#include "../engine/logmanager/logmanager.h"
#include "../engine/nodetree/forms/circle.h"
#include "../engine/nodetree/forms/form.h"

#include "meteors.h"

Meteors::Meteors() : Node("Meteor"), m_velocity(0), m_spinSpeed(0), m_spinDirection(1), m_damage(0), m_spritenode(nullptr), m_collider() {}

void Meteors::Init() {
    Node::Init();

    // setup sprite
    m_spritenode = new Spritenode("../assets/Sprites/ball.png","MeteorSprite");
    AddChild(*m_spritenode);

    // setup collider
    m_collider = new ColliderNode(ft_RECTANGLE);
    m_collider->RegisterOnEnter<Meteors>(&Meteors::OnImpact, *this);
    AddChild(*m_collider);
}

void Meteors::Process(float deltaTime) {
    Node::Process(deltaTime);

    m_collider->m_transform->scale = m_transform->scale;
    m_collider->m_transform->height = m_transform->height;
    m_collider->m_transform->width = m_transform->width;

    m_spritenode->m_transform->scale = m_transform->scale;
    m_spritenode->m_transform->height = m_transform->height;
    m_spritenode->m_transform->width = m_transform->width;
    m_position->y += m_velocity*deltaTime;
    Spin(deltaTime);
}



void Meteors::Spin(const float deltaTime) const {
    m_spritenode->m_transform->rotation += (m_spinSpeed * deltaTime) * static_cast<float>(m_spinDirection);
    m_spritenode->m_transform->scale = m_transform->scale;
}

void Meteors::Draw(Renderer &renderer) {
    Node::Draw(renderer);
}

void Meteors::OnImpact(const Node* e) {
    LogManager::GetInstance().Log(INFO, "EINGESCHLAGEN! in %s", e->name.c_str());
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



