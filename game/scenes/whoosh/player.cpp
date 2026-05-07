#include "player.h"

#include "imgui.h"
#include "../../../lib/BOX2D/src/body.h"
#include "../../../lib/BOX2D/src/sensor.h"
#include "../../config/config.h"
#include "../../engine/input/input.h"
#include "../../engine/logmanager/logmanager.h"
#include "../../engine/nodes/nodefactory.h"
#include "../../engine/physics/physicsmanager.h"
#include "../../engine/scenemanager/scenemanager.h"
#include "box2d/box2d.h"

using namespace Engine;

Player::Player() : m_rigidBody(nullptr), m_bIsGrounded(true), m_bIsFlipped(false),
                   m_fGroundAcceleration(0), m_fGroundDeceleration(0), m_fGroundMinSpeed(0),
                   m_fGroundMaxSpeed(0), m_jumpsMade(0), m_maxJumps(1), m_fJumpForce(0), m_currentAnimation(nullptr), m_groundSensor(nullptr) {
    m_bHasFiredHook = false;
    m_bIsShooting = false;
    m_bCheatsEnabled = false;
    m_hasLevelCompleted = false;


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
            "MovementCheats", [](Node &n) {
                if (auto *s = dynamic_cast<Player *>(&n)) {
                    ImGui::Checkbox("##Editor", &s->m_bCheatsEnabled);
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
    m_groundSensor = dynamic_cast<ColliderNode *>(GetChild("GroundSensor"));
    if (m_groundSensor != nullptr) {
        m_groundSensor->OnEntry.Register<Player>(&Player::OnLandOnGround, *this);
        m_groundSensor->OnExit.Register<Player>(&Player::OnJump, *this);
    }


    m_levelGoal = dynamic_cast<ColliderNode*>(GetChild("GoalSensor"));
    if (m_levelGoal != nullptr) {
        m_levelGoal->OnEntry.Register<Player>(&Player::OnFinishLevel, *this);
    }

}


void Player::Process(float deltaTime) {
    Node::Process(deltaTime);
    if (m_rigidBody == nullptr)
        return;

    if (m_bCheatsEnabled) {
        HandleMovementCheat(deltaTime);
    }
    else {
        HandleMovement(deltaTime);
        HandleHookControls();
        HandleHookVelocity();
        HandleAnimations();
        HandleFlip();
    }
}

void Player::HandleMovementCheat(float deltaTime) {

    auto pos = m_rigidBody->GetBodyPosition();
    float flySpeed = 500 * deltaTime;
    if (InputManager::GetInstance().GetButtonState(SDLK_d) == BS_HELD) {
        pos.x += flySpeed;
        m_rigidBody->SetPositionInMeters(PhysicsManager::PixelsToMeterVector(pos));
    }

    if (InputManager::GetInstance().GetButtonState(SDLK_a) == BS_HELD) {
        pos.x -= flySpeed;
        m_rigidBody->SetPositionInMeters(PhysicsManager::PixelsToMeterVector(pos));
    }

    if (InputManager::GetInstance().GetButtonState(SDLK_w) == BS_HELD) {
        pos.y -= flySpeed;
        m_rigidBody->SetPositionInMeters(PhysicsManager::PixelsToMeterVector(pos));
    }

    if (InputManager::GetInstance().GetButtonState(SDLK_s) == BS_HELD) {
        pos.y += flySpeed;
        m_rigidBody->SetPositionInMeters(PhysicsManager::PixelsToMeterVector(pos));
    }
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

    if (!m_bHasFiredHook) {
        m_rigidBody->SetHorizontalVelocity(m_velocity);
    }
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


void Player::HandleHookControls() {
    if (!m_bHasFiredHook) {
        Vector2d mousePosition = InputManager::GetInstance().GetMousePosition()  + m_globalTransform.position;

        // temp solution!!!!!!!!!!!!! (assumes the player is always in the center of the camera)
        // Problem: the mouse click position is not transformed when the ortho matrix is transformed that sets the offset for the camera
        auto mouseOffset = Vector2d(Config::GetInstance().windowsWidth/2, Config::GetInstance().windowsHeight/2);
        mousePosition -= mouseOffset;

        // check button pressed
        if (InputManager::GetInstance().GetCurrentMouseEvent().button == 1) {
            if (InputManager::GetInstance().GetCurrentMouseEvent().type == SDL_MOUSEBUTTONDOWN && m_bIsShooting == false) {
                m_bIsShooting = true;

                if (!ShootHookSwing(mousePosition)) {
                    m_bIsShooting = false;
                    return;
                }
            }
            if (InputManager::GetInstance().GetCurrentMouseEvent().type == SDL_MOUSEBUTTONUP && m_bIsShooting == true) {
                m_bIsShooting = false;
                m_bHasFiredHook = true;
            }
        }
        if (InputManager::GetInstance().GetCurrentMouseEvent().button == 3 && InputManager::GetInstance().GetCurrentMouseEvent().type == SDL_MOUSEBUTTONUP) {
            ShootHookPull(mousePosition);
        }

    }
    else {
        if (InputManager::GetInstance().GetCurrentMouseEvent().type == SDL_MOUSEBUTTONDOWN && m_bIsShooting == false) {
            m_bIsShooting = true;
            if (b2Joint_IsValid(m_b2Hook)) {
                b2DestroyJoint(m_b2Hook, false);
            }
        }
        if (InputManager::GetInstance().GetCurrentMouseEvent().type == SDL_MOUSEBUTTONUP && m_bIsShooting == true) {
            m_bIsShooting = false;
            m_bHasFiredHook = false;
        }
    }
}

void Player::HandleHookVelocity() {
    if (!m_bHasFiredHook) return;
    if (!b2Joint_IsValid(m_b2Hook)) return;

    auto currentVelocity = m_rigidBody->GetBodyVelocity();

    // accelerate while falling (y velocity > 0) and direction is pressed
    if (m_rigidBody->GetBodyVelocity().y > 0) {
        if (m_rigidBody->GetBodyVelocity().x > 0 && InputManager::GetInstance().GetButtonState(SDLK_d) == BS_HELD) {
            m_rigidBody->SetHorizontalVelocity( Vector2d(currentVelocity.x + 0.01, currentVelocity.y ));

        }
        else if (m_rigidBody->GetBodyVelocity().x > 0) {
            m_rigidBody->SetHorizontalVelocity( Vector2d(currentVelocity.x * 0.999, currentVelocity.y ));

        }
        if (m_rigidBody->GetBodyVelocity().x < 0 && InputManager::GetInstance().GetButtonState(SDLK_a) == BS_HELD) {
            m_rigidBody->SetHorizontalVelocity( Vector2d(currentVelocity.x - 0.01, currentVelocity.y ));
        }
        else if (m_rigidBody->GetBodyVelocity().x < 0) {
            m_rigidBody->SetHorizontalVelocity( Vector2d(currentVelocity.x * 0.999, currentVelocity.y ));
        }
    }


    // decelerate by factor while rising (y velocity < 0) and nothing is pressed


}


bool Player::ShootHookSwing(Vector2d posInPixel) {

    b2WorldId world = PhysicsManager::GetInstance().GetWorld();
    Vector2d playerPositionInBox2DWorld = PhysicsManager::PixelsToMeterVector(GetGlobalPosition() + 80);
    Vector2d targetPositionInBox2DWorld = PhysicsManager::PixelsToMeterVector(posInPixel);

    b2RayResult rayResult = CastRayFromTo(playerPositionInBox2DWorld, targetPositionInBox2DWorld, world);
    if (!rayResult.hit) return false;

    // body that got hit
    b2BodyId targetBodyId = b2Shape_GetBody(rayResult.shapeId);

    CreateChainBetween(playerPositionInBox2DWorld,rayResult.point, targetBodyId, world);
    return true;
}

bool Player::ShootHookPull(Vector2d posInPixel) const {
    b2WorldId world = PhysicsManager::GetInstance().GetWorld();
    Vector2d playerPositionInBox2DWorld = PhysicsManager::PixelsToMeterVector(GetGlobalPosition() + 80);
    Vector2d targetPositionInBox2DWorld = PhysicsManager::PixelsToMeterVector(posInPixel);

    b2RayResult rayResult = CastRayFromTo(playerPositionInBox2DWorld, targetPositionInBox2DWorld, world);
    if (!rayResult.hit) return false;


    m_rigidBody->ResetBodyVelocity();
    m_rigidBody->ApplyImpluseToCenter((playerPositionInBox2DWorld - targetPositionInBox2DWorld) * 5);
    return true;
}


float RayCastCallback(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void *context) {

    auto attachedBody = b2Shape_GetBody(shapeId);
    auto rayResult = static_cast<b2RayResult*>(context);
    if (rayResult == nullptr) return -1;

    if (b2Body_GetType(attachedBody) != b2_staticBody) {
        rayResult->hit = false;
        return 1;
    }
    if (b2Shape_IsSensor(shapeId) == true) {
        rayResult->hit = false;
        return 1;
    }

    rayResult->shapeId = shapeId;
    rayResult->point = point;
    rayResult->fraction = fraction;
    rayResult->normal = normal;
    rayResult->hit = true;
    return fraction;
}

b2RayResult Player::CastRayFromTo(Vector2d origin, Vector2d target, b2WorldId world) const {

    // Calculate ray points
    const b2Vec2 translation = (origin - target) * 8;

    // Set up query filter
    const b2QueryFilter filter = b2DefaultQueryFilter();

    auto* rayResult = new b2RayResult();

    // Cast ray and get closest hit
    b2World_CastRay(world, origin, translation, filter, RayCastCallback, rayResult);

    return *rayResult;
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


    b2JointDef playerJointDef = b2JointDef();
    playerJointDef.bodyIdA = m_rigidBody->GetBodyId();
    playerJointDef.bodyIdB = targetBody;
    playerJointDef.localFrameA = localTransformPlayer;
    playerJointDef.localFrameB = localTransformTarget;
    playerJointDef.collideConnected = true;

    b2DistanceJointDef distanceJointDef = b2DefaultDistanceJointDef();
    distanceJointDef.base = playerJointDef;
    distanceJointDef.enableSpring = true;
    distanceJointDef.enableLimit = true;
    distanceJointDef.hertz = 0.8f;
    distanceJointDef.dampingRatio = 3.f;
    distanceJointDef.minLength = 0;
    distanceJointDef.maxLength = 10;

    auto distanceJoint = b2CreateDistanceJoint(PhysicsManager::GetInstance().GetWorld(), &distanceJointDef);
    m_b2Hook = distanceJoint;
}

void Player::OnJump(const b2ShapeId* target) {
    m_bIsGrounded = false;
}

void Player::OnLandOnGround(const b2ShapeId* target) {
    m_bIsGrounded = true;
    m_jumpsMade = 0;
}

void Player::OnFinishLevel(const b2ShapeId* collidedShapes) {
    if (collidedShapes == nullptr) return;
    if (!b2Shape_IsValid(collidedShapes[0])) return;
    if (!b2Shape_IsSensor(collidedShapes[0])) return;

    if (b2Body_GetType(b2Shape_GetBody(collidedShapes[0])) == b2_staticBody) {
        SceneManager::GetInstance().SetSceneActive("MainMenu");
    };

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
