#include "bouncingball.h"

#include "../../config/config.h"
#include "../../engine/logmanager/logmanager.h"
#include "../../helper/inlinehelper.h"


BouncingBall::BouncingBall() : m_ballSprite(nullptr) {}
BouncingBall::~BouncingBall() = default;

void BouncingBall::Init() {
    Node::Init();
    // ignore parent transform;
    m_globalTransformationFlag = IF_Disable;

    const float MAX_SPEED = 250.f;
    const int SCREEN_WIDTH =  Config::GetInstance().windowsWidth;
    const int SCREEN_HEIGHT =  Config::GetInstance().windowsHeight;


    m_globalTransform.SetSize(100,100);
    RandomizeSize();

    m_globalTransform.position.x = static_cast<float>(GetRandomInt(m_globalTransform.GetWidth() / 2, SCREEN_WIDTH - m_globalTransform.GetWidth() / 2));
    m_globalTransform.position.y = static_cast<float>(GetRandomInt(m_globalTransform.GetHeight() / 2, SCREEN_HEIGHT - m_globalTransform.GetHeight() / 2));
    m_velocity.x = GetRandomPercentage() * static_cast<float>(GetRandomPosOrNeg()) * MAX_SPEED;
    m_velocity.y = GetRandomPercentage() * static_cast<float>(GetRandomPosOrNeg()) * MAX_SPEED;

    ComputeBoundary(SCREEN_HEIGHT, SCREEN_WIDTH);

    m_ballSprite = new SpriteNode();
    m_ballSprite->SetSpritePath(std::string("../assets/Sprites/ball.png").data());
    m_ballSprite->m_spriteDisplayMode = Fit;
    AddChild(*m_ballSprite);
    RandomizeColor();
}

void BouncingBall::Process(float deltaTime) {
    Node::Process(deltaTime);

    if (m_globalTransform.position.x >= m_boundaryWidth.y) {
        m_velocity.x *= -1;
        m_globalTransform.position.x = m_boundaryWidth.y;
        RandomizeColor();
    }
    else if (m_globalTransform.position.x <= m_boundaryWidth.x) {
        m_velocity.x *= -1;
        m_globalTransform.position.x = m_boundaryWidth.x;
        RandomizeColor();
    }
    else if (m_globalTransform.position.y >= m_boundaryHeight.y) {
        m_velocity.y *= -1;
        m_globalTransform.position.y = m_boundaryHeight.y;
        RandomizeColor();
    }
    else if (m_globalTransform.position.y <= m_boundaryHeight.x) {
        m_velocity.y *= -1;
        m_globalTransform.position.y = m_boundaryHeight.x;
        RandomizeColor();
    }

    m_globalTransform.position += m_velocity * deltaTime;
}


void BouncingBall::ComputeBoundary(const int s_height, const int s_width) {
    m_boundaryHeight.x = m_globalTransform.GetHeight() / 2;
    m_boundaryHeight.y = static_cast<float>(s_height) - m_globalTransform.GetHeight() / 2;
    m_boundaryWidth.x = m_globalTransform.GetWidth() / 2;
    m_boundaryWidth.y = static_cast<float>(s_width) - m_globalTransform.GetWidth() / 2;
}

void BouncingBall::RandomizeColor() {
    m_ballSprite->SetRGBA(GetRandomPercentage(), GetRandomPercentage(), GetRandomPercentage(), 1);
}


void BouncingBall::RandomizeSize() {
    float p = 0;
    do {
        p = GetRandomPercentage();
    } while (p == 0);
    m_globalTransform.SetScale(p);
}
