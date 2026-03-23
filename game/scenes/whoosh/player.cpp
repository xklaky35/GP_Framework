
#include "player.h"

#include "../../engine/input/input.h"
#include "../../engine/logmanager/logmanager.h"


using namespace Engine;

Player::Player() : m_spriteNode(nullptr), m_rigidBody(nullptr), m_bIsGrounded(true), m_fGroundAcceleration(200),
                   m_fGroundDeceleration(400),
                   m_fGroundMaxSpeed(400), m_jumpsMade(0), m_maxJumps(1) {
}

Player::~Player() = default;

void Player::Init() {
    Node::Init();
    m_globalTransformationFlag = Disable;

    m_globalTransform.SetSize(100,100);
    m_globalTransform.position = Vector2d(100,100);

    m_spriteNode = new SpriteNode("../assets/Sprites/Shinobi/sprite_frame1.png");
    m_spriteNode->m_spriteDisplayMode = Scale;
    AddChild(*m_spriteNode);

    m_rigidBody = new RigidbodyNode(b2_dynamicBody);
    AddChild(*m_rigidBody);

}

void Player::Process(float deltaTime) {
    Node::Process(deltaTime);
    HandleMovement(deltaTime);
}


void Player::HandleMovement(float deltaTime) {
    LogManager::GetInstance().Log(INFO, "Horizontal: (%f)  Vertical: (%f)", m_velocity.x, m_velocity.y);

    if (InputManager::GetCurrentEvents().GetButtonState(SDLK_d)) {
        if (m_velocity.x < m_fGroundMaxSpeed) {
            m_moveDirection.x = 1;
            m_velocity += m_moveDirection * m_fGroundAcceleration * deltaTime;
        }
    }
    else {
        if (m_velocity.x > 0) {
            m_velocity.x -= m_fGroundDeceleration * deltaTime;
        }
    }

    if (InputManager::GetCurrentEvents().GetButtonState(SDLK_a)) {
        if (abs(m_velocity.x) < m_fGroundMaxSpeed) {
            m_moveDirection.x = -1;
            m_velocity += m_moveDirection * m_fGroundAcceleration * deltaTime;
        }
    }
    else {
        if (m_velocity.x < 0) {
            m_velocity.x += m_fGroundDeceleration * deltaTime;
        }
    }

    if (InputManager::GetCurrentEvents().GetButtonState(SDLK_SPACE)) {
        if (m_jumpsMade < m_maxJumps) {
            m_rigidBody->AddForceToCenter(Vector2d(0,-50));
            m_jumpsMade++;
        }
    }

    //m_deltaSum += deltaTime;
    m_globalTransform.position.x += m_velocity.x * deltaTime;
    m_globalTransform.position.y += m_velocity.y * deltaTime;
}


void Player::OnLandOnGround() {
    m_bIsGrounded = true;
    m_jumpsMade = 0;
}


