#ifndef GP_FRAMEWORK_PLAYER_H
#define GP_FRAMEWORK_PLAYER_H
#include "../../engine/nodes/node.h"
#include "../../engine/nodes/nodefactory.h"
#include "../../engine/nodes/rigidbodynode.h"

using namespace Engine;
class Player : public Node {

public:
    Player();

    void Init() override;

    void HandleAnimations();

    void HandleFlip();

    b2RayResult CastRayFromTo(Vector2d origin, Vector2d target, b2WorldId worldId) const;

    void CreateChainBetween(b2Vec2 vector2d, b2Vec2 point, b2BodyId targetBody, b2WorldId worldId);

    void OnJump(const b2ShapeId *target);

    void ShootHook(Vector2d pos);

    void HandleHookControls();

    void HandleHookVelocity();

    void Process(float deltaTime) override;
    void HandleMovement(float deltaTime);
    void OnLandOnGround(const b2ShapeId* target);

    void ChangeAnimation(AnimatedSpriteNode *animation);

    void SetupParameter(IniParser *parser, const std::string &section) override;

public:
    RigidbodyNode* m_rigidBody;


    // Ground
    bool m_bIsGrounded;
    bool m_bIsFlipped;
    float m_fGroundAcceleration;
    float m_fGroundDeceleration;
    float m_fGroundMinSpeed;
    float m_fGroundMaxSpeed;
    float m_fJumpForce;

    int m_jumpsMade;
    int m_maxJumps;

    Vector2d m_velocity;
    Vector2d m_moveDirection;


    bool m_bHasFiredHook;
    b2JointId m_b2Hook;

private:

    AnimatedSpriteNode* m_currentAnimation;
    AnimatedSpriteNode* m_runningAnimation;
    AnimatedSpriteNode* m_idleAnimation;
    AnimatedSpriteNode* m_jumpAnimation;
    AnimatedSpriteNode* m_walkingAnimation;
    ColliderNode* m_groundSensor;

};
REGISTER_CLASS(Player);

#endif //GP_FRAMEWORK_PLAYER_H