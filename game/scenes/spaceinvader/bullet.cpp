
#include "bullet.h"

using namespace Engine;

Bullet::Bullet(const float speed, const float ttl, const Vector2d dir, const Vector2d pos) : m_dir(dir), m_speed(speed), TTL(ttl), m_pos(pos) {
    m_pSpritePath = "../assets/Sprites/ball.png";
    m_name  = "Bullet";
}


void Bullet::Init() {
    SpriteNode::Init();
    m_globalTransformationFlag = Disable;
    m_spriteDisplayMode = Fit;
    m_globalTransform.SetHeight(10);
    m_globalTransform.position = m_pos;
}

void Bullet::Process(float deltaTime) {
    SpriteNode::Process(deltaTime);

    m_globalTransform.position += m_dir * m_speed * deltaTime;
}
