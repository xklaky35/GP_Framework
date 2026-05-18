#ifndef GP_FRAMEWORK_PLAYER_H
#define GP_FRAMEWORK_PLAYER_H
#include "fmod.hpp"
#include "../../engine/nodes/node.h"
#include "../../engine/nodes/nodefactory.h"
#include "../../engine/nodes/rigidbodynode.h"

using namespace Engine;
class Player : public Node {

public:
    Player();
    ~Player() override;


    void Init() override;

    void Process(float deltaTime) override;
    void SetupParameter(IniParser *parser, const std::string &section) override;

    float GetHookMinRange() const;
    float GetHookMaxRange() const;
    float GetHookDamping() const;
    float GetHookHertz() const;
    float GetHookSwingAcceleration() const;
    float GetHookSwingDeceleration() const;
    bool GetHookSpringEnabled() const;

    void SetHookMinRange(float);
    void SetHookMaxRange(float);
    void SetHookDamping(float);
    void SetHookHertz(float);
    void SetHookSwingAcceleration(float);
    void SetHookSwingDeceleration(float);
    void SetHookSpringEnabled(bool);



private:
    void DestroyHook();

    void HandleFlip() const;
    void HandleAnimations();
    void HandleSound();
    void HandleHookControls();
    void HandleHookVelocity() const;
    void HandleHookVisualisation() const;
    void HandleMovementCheat(float deltaTime) const;
    void HandleMovement(float deltaTime);
    void HandleCollision(const b2ShapeId*);

    void Reset(Vector2d pos);


    void OnHitWallRight(const b2ShapeId *target);
    void OnHitWallLeft(const b2ShapeId *target);
    void OnLandOnGround(const b2ShapeId* target);

    void ChangeAnimation(AnimatedSpriteNode *animation);
    bool ShootHookSwing(Vector2d pos);
    void CreateChainBetween(b2Vec2 vector2d, b2Vec2 point, b2BodyId targetBody, b2WorldId worldId);
    [[nodiscard]] b2RayResult CastRayFromTo(Vector2d origin, Vector2d target, b2WorldId worldId) const;



public:

    bool m_bHasTargetObjectReceived;

private:
    SDL_MouseButtonEvent m_lastMouseButtonEvent;


    // Ground stats
    bool m_bIsGrounded;

    float m_fGroundAcceleration;
    float m_fGroundDeceleration;
    float m_fGroundMinSpeed;
    float m_fGroundMaxSpeed;
    float m_fJumpForce;


    // air stats
    float m_fMaxSwingSpeed;

    int m_iJumpsMade;
    int m_iMaxJumps;

    Vector2d m_velocity;
    Vector2d m_moveDirection;

    // state flags
    bool m_bIsHookAttached;
    bool m_bCheatsEnabled;
    bool m_bIsHittingWall;
    bool m_bIsFlipped;
    bool m_bHasLevelCompleted;

    // hook stats
    float m_fMinRange;
    float m_fMaxRange;
    float m_fDamping;
    float m_fHertz;
    float m_fSwingAcceleration;
    float m_fSwingDeceleration;
    bool m_bJointSpringEnabled;

    // hook sprite controls
    Vector2d m_globalHookOrigin;
    Vector2d m_globalHookTarget;

    // components
    b2JointId m_b2Hook;
    RigidbodyNode* m_pRigidBody;
    AnimatedSpriteNode* m_pCurrentAnimation;
    AnimatedSpriteNode* m_pRunningAnimation;
    AnimatedSpriteNode* m_pIdleAnimation;
    AnimatedSpriteNode* m_pJumpAnimation;
    AnimatedSpriteNode* m_pWalkingAnimation;
    ColliderNode* m_pGroundSensor;
    ColliderNode* m_pWallSensorLeft;
    ColliderNode* m_pWallSensorRight;
    ColliderNode* m_pLevelGoal;
    SpriteNode* m_pHookSprite;

    FMOD::Channel* m_pWalkingSound;
    FMOD::Channel* m_pLandingSound;

};
REGISTER_CLASS(Player);

#endif //GP_FRAMEWORK_PLAYER_H