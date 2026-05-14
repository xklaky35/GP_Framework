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


    float GetHookMinRange();
    float GetHookMaxRange();
    float GetHookDamping();
    float GetHookHertz();
    float GetHookSwingAcceleration();
    float GetHookSwingDeceleration();
    bool GetHookSpringEnabled();

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
    b2RayResult CastRayFromTo(Vector2d origin, Vector2d target, b2WorldId worldId) const;



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

    int m_jumpsMade;
    int m_maxJumps;

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
    RigidbodyNode* m_rigidBody;
    AnimatedSpriteNode* m_currentAnimation;
    AnimatedSpriteNode* m_runningAnimation;
    AnimatedSpriteNode* m_idleAnimation;
    AnimatedSpriteNode* m_jumpAnimation;
    AnimatedSpriteNode* m_walkingAnimation;
    ColliderNode* m_groundSensor;
    ColliderNode* m_wallSensorLeft;
    ColliderNode* m_wallSensorRight;
    ColliderNode* m_levelGoal;
    SpriteNode* m_hookSprite;

    FMOD::Channel* m_walkingSound;
    FMOD::Channel* m_landingSound;

};
REGISTER_CLASS(Player);

#endif //GP_FRAMEWORK_PLAYER_H