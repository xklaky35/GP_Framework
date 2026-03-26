#ifndef GP_FRAMEWORK_RECTANGLE_H
#define GP_FRAMEWORK_RECTANGLE_H

#include "bulletspawner.h"
#include "../../engine/nodes/node.h"
#include "../../engine/nodes/spritenode.h"
#include "../../engine/nodes/collidernode.h"
#include "../../engine/nodes/rigidbodynode.h"

using namespace Engine;

class Spaceship : public Node {
public:
    Spaceship();

    void Init() override;
    void OnCollision(const Node *node);
    void HandleMovement(float);
    void Process(float deltaTime) override;
    void Draw(Renderer &) override;

public:
    Vector2d direction;
    Vector2d new_velocity;
    float m_accellerationFactor;
    float m_breakFactor;
    float m_maxSpeed;
    float m_rotationSpeed;
    float m_dps;
    float m_deltaSum;
    bool m_isPressed;

    // child setup
    SpriteNode* m_spriteNode;
    ColliderNode* m_colliderNode;
    BulletSpawner* m_bulletSpawner;
    RigidbodyNode* m_rigidBody;

};

#endif //GP_FRAMEWORK_RECTANGLE_H