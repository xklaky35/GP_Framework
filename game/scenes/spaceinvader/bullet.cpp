
#include "bullet.h"


using namespace Engine;

Bullet::Bullet(const float speed, const float ttl, const Vector2d dir, const Vector2d pos) : m_dir(dir), m_speed(speed),
    m_ttl(ttl), m_deltaSum(0), m_colliderNode(nullptr), m_pos(pos) {
    m_spritePath = "../assets/Sprites/ball.png";
    m_name = "Bullet";
}


void Bullet::Init() {
    SpriteNode::Init();
    m_globalTransformationFlag = IF_Disable;
    m_spriteDisplayMode = Scale;
    m_globalTransform.SetHeight(10);
    m_globalTransform.SetWidth(10);
    m_globalTransform.position = m_pos + m_dir;

    m_colliderNode = new ColliderNode();
    m_colliderNode->m_globalTransform.SetHeight(m_globalTransform.GetHeight());
    m_colliderNode->m_globalTransform.SetWidth(m_globalTransform.GetHeight());

    m_colliderNode->m_globalTransform.position.x += m_globalTransform.GetWidth() / 2;
    m_colliderNode->m_globalTransform.position.y += m_globalTransform.GetHeight() / 2;
    m_colliderNode->m_name = "Bullet";
    AddChild(*m_colliderNode);

}

void Bullet::Process(float deltaTime) {
    SpriteNode::Process(deltaTime);

    m_globalTransform.position += m_dir * m_speed * deltaTime;
    m_deltaSum += deltaTime;

    // apply ttl
    if (m_deltaSum >= m_ttl) {
        m_parent->RemoveChild(this);
    }
}
