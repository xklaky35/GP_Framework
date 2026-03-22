#include "../../engine/nodes/collidernode.h"
#include "spaceship.h"

#include "../../engine/input/input.h"
#include "../../engine/logmanager/logmanager.h"

Spaceship::Spaceship(float max_speed, const char* nodeName) : Node(nodeName), m_accellerationFactor(0.4),
                                                              m_breakFactor(0.1),
                                                              m_maxSpeed(500),
                                                              m_rotationSpeed(0.3),
                                                              m_isPressed(false),
                                                              m_spriteNode(nullptr), m_colliderNode(nullptr) {
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
    AddChild(*m_bulletSpawner);

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
            new_velocity.x -= m_accellerationFactor;
        } else {
            new_velocity.x += m_accellerationFactor;
        }
        if (new_velocity.y > maxNewVelocity.y) {
            new_velocity.y -= m_accellerationFactor;
        } else {
            new_velocity.y += m_accellerationFactor;
        }
    }
    else {
        if (new_velocity.x > 0) {
            new_velocity.x -= m_breakFactor;
        } else {
            new_velocity.x += m_breakFactor;
        }
        if (new_velocity.y > 0) {
            new_velocity.y -= m_breakFactor;
        } else {
            new_velocity.y += m_breakFactor;
        }
    }


    if (InputManager::GetCurrentEvents().GetButtonState(SDLK_a)) {
        m_globalTransform.SetRotation(m_globalTransform.GetRotationDeg() + m_rotationSpeed);
        const Vector2d vecX(cos(m_globalTransform.GetRotationRad()), sin(m_globalTransform.GetRotationRad()));
        const Vector2d vecY(-sin(m_globalTransform.GetRotationRad()), -cos(m_globalTransform.GetRotationRad()));
        direction = ((vecX * 0) + (vecY * 1));
    }


    if (InputManager::GetCurrentEvents().GetButtonState(SDLK_d)) {
        m_globalTransform.SetRotation(m_globalTransform.GetRotationDeg() - m_rotationSpeed);
        const Vector2d vecX(cos(m_globalTransform.GetRotationRad()), sin(m_globalTransform.GetRotationRad()));
        const Vector2d vecY(-sin(m_globalTransform.GetRotationRad()), -cos(m_globalTransform.GetRotationRad()));
        direction = ((vecX * 0) + (vecY * 1));
    }

    if (InputManager::GetCurrentEvents().GetButtonState(SDLK_SPACE)) {
        m_bulletSpawner->ShootInDirection(direction);
    }

    m_globalTransform.position += new_velocity * deltaTime;
}

void Spaceship::Draw(Renderer &renderer) {
    Node::Draw(renderer);
}
