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
    void Process(float deltaTime) override;
    void SetupParameter(IniParser *parser, const std::string &section) override;


private:
    void HandleFlip();
    void HandleAnimations();
    void HandleHookControls();
    void HandleHookVelocity();
    void HandleMovementCheat(float deltaTime);
    void HandleMovement(float deltaTime);

    void OnJump(const b2ShapeId *target);
    void OnLandOnGround(const b2ShapeId* target);
    void OnFinishLevel(const b2ShapeId*);

    void ChangeAnimation(AnimatedSpriteNode *animation);
    bool ShootHookSwing(Vector2d pos);
    bool ShootHookPull(Vector2d vector2d) const;
    void CreateChainBetween(b2Vec2 vector2d, b2Vec2 point, b2BodyId targetBody, b2WorldId worldId);
    b2RayResult CastRayFromTo(Vector2d origin, Vector2d target, b2WorldId worldId) const;


public:
    RigidbodyNode* m_rigidBody;


    // Ground
    bool m_bIsGrounded;
    bool m_bIsFlipped;
    bool m_hasLevelCompleted;
    float m_fGroundAcceleration;
    float m_fGroundDeceleration;
    float m_fGroundMinSpeed;
    float m_fGroundMaxSpeed;
    float m_fJumpForce;


    // air
    float m_fMaxSwingSpeed;
    float m_fSwingAcceleration;
    float m_fSwingDeceleration;

    int m_jumpsMade;
    int m_maxJumps;

    Vector2d m_velocity;
    Vector2d m_moveDirection;


    bool m_bHasFiredHook;
    b2JointId m_b2Hook;
    bool m_bIsShooting;
    bool m_bCheatsEnabled;

private:

    AnimatedSpriteNode* m_currentAnimation;
    AnimatedSpriteNode* m_runningAnimation;
    AnimatedSpriteNode* m_idleAnimation;
    AnimatedSpriteNode* m_jumpAnimation;
    AnimatedSpriteNode* m_walkingAnimation;
    ColliderNode* m_groundSensor;
    ColliderNode* m_levelGoal;

};
REGISTER_CLASS(Player);

#endif //GP_FRAMEWORK_PLAYER_H