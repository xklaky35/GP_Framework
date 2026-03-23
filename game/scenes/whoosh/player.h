#ifndef GP_FRAMEWORK_PLAYER_H
#define GP_FRAMEWORK_PLAYER_H
#include "../../engine/nodes/node.h"
#include "../../engine/nodes/rigidbodynode.h"
#include "../../engine/nodes/spritenode.h"


using namespace Engine;
class Player : public Node {

public:
    Player();
    ~Player();

    void Init() override;
    void Process(float deltaTime) override;
    void HandleMovement(float deltaTime);
    void OnLandOnGround();

public:
    SpriteNode* m_spriteNode;
    RigidbodyNode* m_rigidBody;


    // Ground
    bool m_bIsGrounded;
    float m_fGroundAcceleration;
    float m_fGroundDeceleration;
    float m_fGroundMaxSpeed;

    int m_jumpsMade;
    int m_maxJumps;

    Vector2d m_velocity;
    Vector2d m_moveDirection;

};


#endif //GP_FRAMEWORK_PLAYER_H