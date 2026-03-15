#include "bouncingball.h"

#include "../../config/config.h"
#include "../../helper/inlinehelper.h"


BouncingBall::BouncingBall() : m_ballSprite(nullptr) {}
BouncingBall::~BouncingBall() = default;

void BouncingBall::Init() {
    Node::Init();

    const float MAX_SPEED = 250.f;
    const int SCREEN_WIDTH =  Config::GetInstance().windowsWidth;
    const int SCREEN_HEIGHT =  Config::GetInstance().windowsHeight;


    RandomizeSize();

    m_transform->height = 100 * m_transform->scale;
    m_transform->width = 100 * m_transform->scale;
    m_position->x = static_cast<float>(GetRandomInt(m_transform->width / 2, SCREEN_WIDTH - m_transform->width / 2));
    m_position->y = static_cast<float>(GetRandomInt(m_transform->height / 2, SCREEN_HEIGHT - m_transform->height / 2));
    m_velocity.x = GetRandomPercentage() * static_cast<float>(GetRandomPosOrNeg()) * MAX_SPEED;
    m_velocity.y = GetRandomPercentage() * static_cast<float>(GetRandomPosOrNeg()) * MAX_SPEED;

    ComputeBoundary(SCREEN_HEIGHT, SCREEN_WIDTH);

    m_ballSprite = new SpriteNode("../assets/Sprites/ball.png", "Ball");
    m_ballSprite->m_bUseSpriteSize = false;
    *m_ballSprite->m_transform = *m_transform;
    AddChild(*m_ballSprite);

    RandomizeColor();
}

void BouncingBall::Process(float deltaTime) {
    Node::Process(deltaTime);

    if (m_position->x >= m_boundaryWidth.y) {
        m_velocity.x *= -1;
        m_position->x = m_boundaryWidth.y;
        RandomizeColor();
    }
    else if (m_position->x <= m_boundaryWidth.x) {
        m_velocity.x *= -1;
        m_position->x = m_boundaryWidth.x;
        RandomizeColor();
    }
    else if (m_position->y >= m_boundaryHeight.y) {
        m_velocity.y *= -1;
        m_position->y = m_boundaryHeight.y;
        RandomizeColor();
    }
    else if (m_position->y <= m_boundaryHeight.x) {
        m_velocity.y *= -1;
        m_position->y = m_boundaryHeight.x;
        RandomizeColor();
    }

    *m_position += m_velocity * deltaTime;
}


void BouncingBall::ComputeBoundary(const int s_height, const int s_width) {
    m_boundaryHeight.x = m_transform->height / 2;
    m_boundaryHeight.y = static_cast<float>(s_height) - m_transform->height / 2;
    m_boundaryWidth.x = m_transform->width / 2;
    m_boundaryWidth.y = static_cast<float>(s_width) - m_transform->width / 2;
}

void BouncingBall::RandomizeColor() {
    m_ballSprite->SetRGBA(GetRandomPercentage(), GetRandomPercentage(), GetRandomPercentage(), 1);
}



void BouncingBall::RandomizeSize() {
    m_transform->scale = GetRandomPercentage();
}
