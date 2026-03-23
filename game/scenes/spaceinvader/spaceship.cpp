#include "../../engine/nodes/collidernode.h"
#include "spaceship.h"

#include "../../../lib/BOX2D/include/box2d/box2d.h"
#include "../../../lib/BOX2D/include/box2d/types.h"
#include "../../engine/input/input.h"
#include "../../engine/logmanager/logmanager.h"
#include "../../engine/sound/soundmanager.h"

Spaceship::Spaceship(float max_speed, const char* nodeName) : Node(nodeName), m_accellerationFactor(200),
                                                              m_breakFactor(70),
                                                              m_maxSpeed(500),
                                                              m_rotationSpeed(300),
                                                              m_dps(5),
                                                              m_deltaSum(0),
                                                              m_isPressed(false),
                                                              m_spriteNode(nullptr), m_colliderNode(nullptr),
                                                              m_bulletSpawner(nullptr), m_rigidBody(nullptr) {
    direction.x = 0;
    direction.y = -1;
}


void Spaceship::Init() {
    Node::Init();
    m_globalTransformationFlag = Disable;

    m_globalTransform.SetSize(200,200);
    // setup sprite
    m_spriteNode = new SpriteNode("../assets/Sprites/spaceship.png");
    m_spriteNode->m_spriteDisplayMode = Scale;
    AddChild(*m_spriteNode);

    // setup collider
    m_colliderNode = new ColliderNode(ft_CIRCLE);
    m_colliderNode->OnCollision.Register<Spaceship>(&Spaceship::OnCollision, *this);
    AddChild(*m_colliderNode);

    m_bulletSpawner = new BulletSpawner();
    m_bulletSpawner->m_transform.position.x += m_globalTransform.GetWidth() / 2;
    m_bulletSpawner->m_transform.position.y += m_globalTransform.GetHeight() / 2;
    AddChild(*m_bulletSpawner);

    m_rigidBody = new RigidbodyNode(b2_dynamicBody);
    AddChild(*m_rigidBody);
}

void Spaceship::OnCollision(const Node* node) {
    //LogManager::GetInstance().Log(INFO, "hit");
}

void Spaceship::Process(float deltaTime) {
    Node::Process(deltaTime);
    HandleMovement(deltaTime);
}

void Spaceship::HandleMovement(float deltaTime) {

    if (InputManager::GetCurrentEvents().GetButtonState(SDLK_w)) {

        Vector2d maxNewVelocity = direction * m_maxSpeed;

        if (new_velocity.x > maxNewVelocity.x) {
            new_velocity.x -= m_accellerationFactor * deltaTime;
        } else {
                new_velocity.x += m_accellerationFactor * deltaTime;
        }
        if (new_velocity.y > maxNewVelocity.y) {
            new_velocity.y -= m_accellerationFactor * deltaTime;
        } else {
            new_velocity.y += m_accellerationFactor * deltaTime;
        }
    }
    else {
        if (new_velocity.x > 0) {
            new_velocity.x -= m_breakFactor * deltaTime;
        } else {
            new_velocity.x += m_breakFactor * deltaTime;
        }
        if (new_velocity.y > 0) {
            new_velocity.y -= m_breakFactor * deltaTime;
        } else {
            new_velocity.y += m_breakFactor * deltaTime;
        }
    }


    if (InputManager::GetCurrentEvents().GetButtonState(SDLK_a)) {
        m_globalTransform.SetRotation(m_globalTransform.GetRotationDeg() + m_rotationSpeed * deltaTime);
        const Vector2d vecX(cos(m_globalTransform.GetRotationRad()), sin(m_globalTransform.GetRotationRad()));
        const Vector2d vecY(-sin(m_globalTransform.GetRotationRad()), -cos(m_globalTransform.GetRotationRad()));
        direction = ((vecX * 0) + (vecY * 1));
    }


    if (InputManager::GetCurrentEvents().GetButtonState(SDLK_d)) {
        m_globalTransform.SetRotation(m_globalTransform.GetRotationDeg() - m_rotationSpeed * deltaTime);
        const Vector2d vecX(cos(m_globalTransform.GetRotationRad()), sin(m_globalTransform.GetRotationRad()));
        const Vector2d vecY(-sin(m_globalTransform.GetRotationRad()), -cos(m_globalTransform.GetRotationRad()));
        direction = ((vecX * 0) + (vecY * 1));
    }

    if (InputManager::GetCurrentEvents().GetButtonState(SDLK_SPACE)) {
        if (m_deltaSum >= 1 / m_dps) {
            SoundManager::GetInstance().Play("hardpop-mainmenu-onSelection.wav");
            m_bulletSpawner->ShootInDirection(direction);
            m_deltaSum = 0;
        }
    }


    if (m_globalTransform.GetRotationDeg() >= 360 || m_globalTransform.GetRotationDeg() <= -360) {
        m_globalTransform.SetRotation(0);
    }

    m_deltaSum += deltaTime;
    m_globalTransform.position += new_velocity * deltaTime;
}

void Spaceship::Draw(Renderer &renderer) {
    Node::Draw(renderer);
}
