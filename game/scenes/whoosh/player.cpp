#include "player.h"

#include "imgui.h"
#include "../../../lib/BOX2D/src/sensor.h"
#include "../../engine/input/input.h"
#include "../../engine/logmanager/logmanager.h"
#include "../../engine/nodes/nodefactory.h"
#include "../../engine/physics/physicsmanager.h"
#include "box2d/box2d.h"

using namespace Engine;

Player::Player() : m_rigidBody(nullptr), m_bIsGrounded(true), m_bIsFlipped(false),
                   m_fGroundAcceleration(0), m_fGroundDeceleration(0), m_fGroundMinSpeed(0),
                   m_fGroundMaxSpeed(0), m_jumpsMade(0), m_maxJumps(1), m_fJumpForce(0), m_currentAnimation(nullptr) {
    m_bHasFiredHook = false;


    SetupNode("Player", NT_Custom);

    m_nodeInfo.push_back(
        {
            "", [](Node &n) {
                ImGui::Separator();
                ImGui::Text("Player Settings");
            }
        });
    m_nodeInfo.push_back({
            "Ground Acceleration", [](Node &n) {
                if (auto *s = dynamic_cast<Player *>(&n)) {
                    int v_min = 0, v_max = 100000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_fGroundAcceleration, 1, 0.5f, &v_min,
                                           &v_max)) {
                        s->SetValue("groundAcceleration", s->m_fGroundAcceleration);
                    }
                }
            }
        }
    );
    m_nodeInfo.push_back({
            "Ground Deceleration", [](Node &n) {
                if (auto *s = dynamic_cast<Player *>(&n)) {
                    int v_min = 0, v_max = 100000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_fGroundDeceleration, 1, 0.5f, &v_min,
                                           &v_max)) {
                        s->SetValue("groundDeceleration", s->m_fGroundDeceleration);
                    }
                }
            }
        }
    );
    m_nodeInfo.push_back({
            "Max ground speed", [](Node &n) {
                if (auto *s = dynamic_cast<Player *>(&n)) {
                    int v_min = 0, v_max = 100000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_fGroundMaxSpeed, 1, 0.5f, &v_min,
                                           &v_max)) {
                        s->SetValue("maxGroundSpeed", s->m_fGroundMaxSpeed);
                    }
                }
            }
        }
    );
    m_nodeInfo.push_back({
            "Max jumps", [](Node &n) {
                if (auto *s = dynamic_cast<Player *>(&n)) {
                    int v_min = 0, v_max = 100000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_U32, &s->m_maxJumps, 1, 0.5f, &v_min, &v_max)) {
                        s->SetValue("maxJumps", s->m_maxJumps);
                    }
                }
            }
        }
    );
    m_nodeInfo.push_back({
            "Jump Force", [](Node &n) {
                if (auto *s = dynamic_cast<Player *>(&n)) {
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_fJumpForce, 1, 0.5f, nullptr, nullptr)) {
                        s->SetValue("jumpForce", s->m_fJumpForce);
                    }
                }
            }
        }
    );
    m_nodeInfo.push_back({
            "Velocity X - Y", [](Node &n) {
                if (auto *s = dynamic_cast<Player *>(&n)) {
                    ImGui::SetNextItemWidth(-FLT_MIN);

                    ImGui::BeginDisabled();
                    ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_velocity, 2, 0.5f, nullptr);
                    ImGui::EndDisabled();
                }
            }
        }
    );
}

void Player::Init() {
    Node::Init();
    m_rigidBody = dynamic_cast<RigidbodyNode *>(GetChild("RigidBody"));

    m_runningAnimation = dynamic_cast<AnimatedSpriteNode *>(GetChild("RunAnimation"));
    if (m_runningAnimation != nullptr) {
        m_runningAnimation->SetRGBA(0,0,0,0);
    }
    m_idleAnimation = dynamic_cast<AnimatedSpriteNode *>(GetChild("IdleAnimation"));
    if (m_idleAnimation != nullptr) {
        m_idleAnimation->SetRGBA(0,0,0,0);
    }
    m_jumpAnimation = dynamic_cast<AnimatedSpriteNode *>(GetChild("JumpAnimation"));
    if (m_jumpAnimation != nullptr) {
        m_jumpAnimation->SetRGBA(0,0,0,0);
    }
    m_walkingAnimation = dynamic_cast<AnimatedSpriteNode *>(GetChild("WalkAnimation"));
    if (m_walkingAnimation != nullptr) {
        m_walkingAnimation->SetRGBA(0,0,0,0);
    }

}


void Player::Process(float deltaTime) {
    Node::Process(deltaTime);
    if (m_rigidBody == nullptr)
        return;


    HandleMovement(deltaTime);
    HandleMouseClick();

    HandleAnimations();
    HandleFlip();
}

void Player::HandleMovement(float deltaTime) {

    if (InputManager::GetInstance().GetButtonState(SDLK_d) == BS_HELD) {
        if (m_bIsFlipped) {
            m_bIsFlipped = false;
        }
        if (m_velocity.x < m_fGroundMaxSpeed) {
            m_moveDirection.x = 1;
            m_velocity += m_moveDirection * m_fGroundAcceleration * deltaTime;
        }
    }
    else {
        if (m_velocity.x > 0) {
            m_velocity.x -= m_fGroundDeceleration * deltaTime;
            if (abs(m_velocity.x) < 0.1) m_velocity.x = 0;
        }
    }

    if (InputManager::GetInstance().GetButtonState(SDLK_a) == BS_HELD) {
        if (!m_bIsFlipped) {
            m_bIsFlipped = true;
        }
        if (abs(m_velocity.x) < m_fGroundMaxSpeed) {
            m_moveDirection.x = -1;
            m_velocity += m_moveDirection * m_fGroundAcceleration * deltaTime;
        }
    }
    else {
        if (m_velocity.x < 0) {
            m_velocity.x += m_fGroundDeceleration * deltaTime;
            if (abs(m_velocity.x) < 0.1) m_velocity.x = 0;
        }
    }

    if (InputManager::GetInstance().GetButtonState(SDLK_SPACE) == BS_PRESSED) {
        if (m_jumpsMade < m_maxJumps) {
            m_rigidBody->ResetBodyVelocity();
            m_rigidBody->ApplyImpluseToCenter(Vector2d(0,-m_fJumpForce));
            m_jumpsMade++;
        }
    }

    m_rigidBody->SetVelocity(m_velocity);
}


void Player::HandleAnimations() {
    float walkingThreshold = m_fGroundMaxSpeed * 0.8f;
    if (abs(m_velocity.x) == 0) {
        ChangeAnimation(m_idleAnimation);
    } else if (abs(m_velocity.x) < walkingThreshold) {
        ChangeAnimation(m_walkingAnimation);
    } else {
        ChangeAnimation(m_runningAnimation);
    }
}

void Player::HandleFlip() {
    if (m_currentAnimation == nullptr) return;

    if (m_currentAnimation->IsFlipped() && !m_bIsFlipped) {
        m_currentAnimation->Flip();
    }
    if (!m_currentAnimation->IsFlipped() && m_bIsFlipped) {
        m_currentAnimation->Flip();
    }
}

b2RayResult Player::CastRayFromTo(Vector2d origin, Vector2d target, b2WorldId world) {

    Vector2d shootingDirection = target - origin;

    float angle = atan2(shootingDirection.y, shootingDirection.x); //* (180/std::numbers::pi);

    // Calculate ray points
    const float rayLength = 10000;
    const b2Vec2 b2origin = b2Vec2(origin.x, origin.y);
    const b2Vec2 translation = b2Vec2(
        rayLength * sin(angle),
        rayLength * cos(angle)
    );

    // Set up query filter
    const b2QueryFilter filter = b2DefaultQueryFilter();

    // Cast ray and get closest hit
    const b2RayResult result = b2World_CastRayClosest(world, b2origin, translation, filter);

    LogManager::GetInstance().Log(INFO, "Mouse position: [ %f : %f ]", target.x, target.y);
    LogManager::GetInstance().Log(INFO, "Player pos: [ %f : %f ]", m_globalTransform.position.x, m_globalTransform.position.y);
    LogManager::GetInstance().Log(INFO, "Target vector: [ %f : %f ]", shootingDirection.x, shootingDirection.y);
    LogManager::GetInstance().Log(INFO, "%f", angle * (180/std::numbers::pi));

    return result;
}

void Player::CreateChainBetween(b2Vec2 origin, b2Vec2 target, b2BodyId targetBody, b2WorldId world) {

    // local point of raycast source
    b2Transform localTransformPlayer;
    localTransformPlayer.p = b2Body_GetLocalPoint(m_rigidBody->GetBodyId(), origin);
    localTransformPlayer.q = b2MakeRot(0);

    // transform of point hit
    b2Transform localTransformTarget;
    localTransformTarget.p = b2Body_GetLocalPoint(targetBody,target);
    localTransformTarget.q = b2MakeRot(0);



    // chain part def
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = origin;

    // chain part shape def
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1;

    // polygon def
    b2Polygon boxShape = b2MakeBox(0.4, 0.05);

    // create initial link
    b2BodyId link = b2CreateBody(world, &bodyDef);
    b2ShapeId shape = b2CreatePolygonShape(link, &shapeDef, &boxShape);

    m_hookFoot = link;



    // Set up the common properties of the joint before entering the loop

    // local point of chain piece A
    b2Transform chainJointTransform;
    chainJointTransform.p = b2Vec2(0.6, 0);
    chainJointTransform.q = b2MakeRot(0);


    // chain joint def
    b2JointDef def = b2JointDef();
    def.localFrameA = chainJointTransform;
    def.localFrameB = chainJointTransform;


    b2RevoluteJointDef revoluteJointDef = b2DefaultRevoluteJointDef();
    revoluteJointDef.base = def;

    // uses same definitions to create multiple chain pieces
    for (int i = 0; i < 10; i++) {
        b2BodyId newLink = b2CreateBody(world, &bodyDef);
        b2ShapeId newShape = b2CreatePolygonShape(newLink, &shapeDef, &boxShape);

        def.bodyIdA = link;
        def.bodyIdB = newLink;
        def.localFrameA.p.x *= -1;
        def.localFrameB.p.x *= -1;
        revoluteJointDef.base = def;
        b2CreateRevoluteJoint(world, &revoluteJointDef);

        // update chain pieces
        link = newLink;
    }
    m_hookHead = link;


    // chain foot to player
    b2JointDef chainPlayerJointDef = b2JointDef();
    chainPlayerJointDef.bodyIdA = m_rigidBody->GetBodyId();
    chainPlayerJointDef.bodyIdB = m_hookFoot;
    chainPlayerJointDef.localFrameA = localTransformPlayer;
    chainPlayerJointDef.localFrameB = chainJointTransform;

    // chain head to target
    b2JointDef chainTargetJointDef = b2JointDef();
    chainTargetJointDef.bodyIdA = targetBody;
    chainTargetJointDef.bodyIdB = m_hookHead;
    chainTargetJointDef.localFrameA = localTransformTarget;
    chainTargetJointDef.localFrameB = chainJointTransform;

    revoluteJointDef.base = chainPlayerJointDef;
    b2CreateRevoluteJoint(world, &revoluteJointDef);

    revoluteJointDef.base = chainTargetJointDef;
    b2CreateRevoluteJoint(world, &revoluteJointDef);

    //b2Body_ApplyLinearImpulse(link, b2Vec2{shootingVector.x, shootingVector.y} * 3, b2Vec2{0, 0}, true);
}

void Player::ShootHook(Vector2d posInPixel) {

    b2WorldId world = PhysicsManager::GetInstance().GetWorld();
    Vector2d playerPositionInBox2DWorld = PhysicsManager::PixelsToMeterVector(GetGlobalPosition());
    Vector2d targetPositionInBox2DWorld = PhysicsManager::PixelsToMeterVector(posInPixel);

    b2RayResult rayCastResult = CastRayFromTo(playerPositionInBox2DWorld, targetPositionInBox2DWorld, world);
    if (rayCastResult.hit == false) return;

    // body that got hit
    b2BodyId targetBodyId = b2Shape_GetBody(rayCastResult.shapeId);


    CreateChainBetween(playerPositionInBox2DWorld,rayCastResult.point, targetBodyId, world);

    /*
    b2DistanceJointDef jointDef = b2DefaultDistanceJointDef();
    jointDef.base = def;
    jointDef.enableSpring = true;
    jointDef.enableLimit = true;
    jointDef.hertz = 0.2f;
    //jointDef.dampingRatio = 1.0f;

    jointDef.minLength = 0;
    jointDef.maxLength = 10;

    b2JointId myJointId = b2CreateDistanceJoint(PhysicsManager::GetInstance().GetWorld(), &jointDef);
    */



}


void Player::HandleMouseClick() {
    if (InputManager::GetInstance().GetCurrentMouseEvent().type == SDL_MOUSEBUTTONDOWN && m_bHasFiredHook == false) {
        Vector2d mousePosition = InputManager::GetInstance().GetMousePosition();
        ShootHook(mousePosition);
        m_bHasFiredHook = true;
    }
    if (InputManager::GetInstance().GetCurrentMouseEvent().type == SDL_MOUSEBUTTONUP) {
        m_bHasFiredHook = false;
    }
}

void Player::OnLandOnGround() {
    m_bIsGrounded = true;
    m_jumpsMade = 0;
}

void Player::ChangeAnimation(AnimatedSpriteNode* animation) {
    if (animation == nullptr) return;

    if (m_currentAnimation == nullptr) {
        m_currentAnimation = animation;
    } else {
        m_currentAnimation->SetRGBA(0, 0, 0, 0);
        m_currentAnimation = animation;
    }

    m_currentAnimation->SetRGBA(1, 1, 1, 1);
    m_currentAnimation->SetAnimating(true);
    m_currentAnimation->SetLooping(true);
}

void Player::SetupParameter(IniParser *parser, const std::string &section) {

    Node::SetupParameter(parser, section); // IMORTANT!!

    // own setup of variables
    m_fGroundAcceleration = parser->GetValueAsFloat(section, "groundAcceleration");
    m_fGroundDeceleration = parser->GetValueAsFloat(section, "groundDeceleration");
    m_fGroundMaxSpeed = parser->GetValueAsFloat(section, "maxGroundSpeed");
    m_fJumpForce = parser->GetValueAsFloat(section, "jumpForce");
    m_maxJumps = parser->GetValueAsInt(section, "maxJumps");

    // child setup
    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/whoosh/Player.ini");
}

