#ifndef GP_FRAMEWORK_PLAYER_H
#define GP_FRAMEWORK_PLAYER_H
#include "../../engine/nodes/node.h"
#include "../../engine/nodes/nodefactory.h"
#include "../../engine/nodes/rigidbodynode.h"
#include "../../engine/nodes/spritenode.h"


using namespace Engine;
class Player : public Node {

public:
    Player();

    void Init() override;
    void Process(float deltaTime) override;
    void HandleMovement(float deltaTime);
    void OnLandOnGround();
    void Setup(IniParser *parser, std::string section) override;

public:
    RigidbodyNode* m_rigidBody;


    // Ground
    bool m_bIsGrounded;
    bool m_bIsFlipped;
    float m_fGroundAcceleration;
    float m_fGroundDeceleration;
    float m_fGroundMinSpeed;
    float m_fGroundMaxSpeed;

    int m_jumpsMade;
    int m_maxJumps;

    Vector2d m_velocity;
    Vector2d m_moveDirection;

};
REGISTER_CLASS(Player);

#endif //GP_FRAMEWORK_PLAYER_H