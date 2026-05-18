#include "player.h"

#include "imgui.h"
#include "levelgoal.h"
#include "../../../lib/BOX2D/src/body.h"
#include "../../config/config.h"
#include "../../engine/input/input.h"
#include "../../engine/nodes/nodefactory.h"
#include "../../engine/physics/physicsmanager.h"
#include "../../engine/scenemanager/scenemanager.h"
#include "../../engine/sound/soundmanager.h"
#include "box2d/box2d.h"

using namespace Engine;

Player::Player()
    : m_bIsGrounded(true),
      m_fGroundAcceleration(0),
      m_fGroundDeceleration(0),
      m_fGroundMinSpeed(0),
      m_fGroundMaxSpeed(0),
      m_fJumpForce(0),
      m_iJumpsMade(0),
      m_iMaxJumps(1),
      m_bIsFlipped(false),
      m_pRigidBody(nullptr),
      m_pCurrentAnimation(nullptr),
      m_pGroundSensor(nullptr),
      m_pWalkingSound(nullptr),
      m_pLandingSound(nullptr) {

    m_lastMouseButtonEvent = SDL_MouseButtonEvent();
    Reset({0,0});

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
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_U32, &s->m_iMaxJumps, 1, 0.5f, &v_min, &v_max)) {
                        s->SetValue("maxJumps", s->m_iMaxJumps);
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
    m_nodeInfo.push_back({
            "Hook range min", [](Node &n) {
                if (auto *s = dynamic_cast<Player *>(&n)) {
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    auto minDistance = s->GetHookMinRange();
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &minDistance, 1, 0.5f, nullptr, nullptr)) {
                        s->SetValue("minHookRange", minDistance);
                        s->SetHookMinRange(minDistance);
                    }
                }
            }
        }
    );
    m_nodeInfo.push_back({
            "Hook range max", [](Node &n) {
                if (auto *s = dynamic_cast<Player *>(&n)) {
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    auto maxDistance = s->GetHookMaxRange();
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &maxDistance, 1, 0.5f, nullptr, nullptr)) {
                        s->SetValue("maxHookRange", maxDistance);
                        s->SetHookMaxRange(maxDistance);
                    }
                }
            }
        }
    );
    m_nodeInfo.push_back({
            "Damping", [](Node &n) {
                if (auto *s = dynamic_cast<Player *>(&n)) {
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    auto damping = s->GetHookDamping();
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &damping, 1, 0.5f, nullptr, nullptr)) {
                        s->SetValue("damping", damping);
                        s->SetHookDamping(damping);
                    }
                }
            }
        }
    );
    m_nodeInfo.push_back({
            "Hertz", [](Node &n) {
                if (auto *s = dynamic_cast<Player *>(&n)) {
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    auto hertz = s->GetHookHertz();
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &hertz, 1, 0.5f, nullptr, nullptr)) {
                        s->SetValue("hertz", hertz);
                        s->SetHookHertz(hertz);
                    }
                }
            }
        }
    );
    m_nodeInfo.push_back({
            "Swing accelleration", [](Node &n) {
                if (auto *s = dynamic_cast<Player *>(&n)) {
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    auto acceleration = s->GetHookSwingAcceleration();
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &acceleration, 1, 0.5f, nullptr, nullptr)) {
                        s->SetValue("swingAcceleration", acceleration);
                        s->SetHookSwingAcceleration(acceleration);
                    }
                }
            }
        }
    );
    m_nodeInfo.push_back({
            "Swing deceleration", [](Node &n) {
                if (auto *s = dynamic_cast<Player *>(&n)) {
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    auto deceleration = s->GetHookSwingDeceleration();
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &deceleration, 1, 0.5f, nullptr, nullptr)) {
                        s->SetValue("swingDeceleration", deceleration);
                        s->SetHookSwingDeceleration(deceleration);
                    }
                }
            }
        }
    );
    m_nodeInfo.push_back({
            "Enable spring", [](Node &n) {
                if (auto *s = dynamic_cast<Player *>(&n)) {
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    auto spring = s->GetHookSpringEnabled();
                    if (ImGui::Checkbox("##Editor", &spring)) {
                        s->SetValue("hasSpring", spring);
                        s->SetHookSpringEnabled(spring);
                    }
                }
            }
        }
    );
}

Player::~Player() {
    if (m_pWalkingSound != nullptr) {
        m_pWalkingSound->stop();
        m_pWalkingSound = nullptr;
    }
}


void Player::Init() {
    Node::Init();

    m_pRigidBody = dynamic_cast<RigidbodyNode *>(GetChild("RigidBody"));

    m_pRunningAnimation = dynamic_cast<AnimatedSpriteNode *>(GetChild("RunAnimation"));
    if (m_pRunningAnimation != nullptr) {
        m_pRunningAnimation->SetRGBA(0,0,0,0);
    }
    m_pIdleAnimation = dynamic_cast<AnimatedSpriteNode *>(GetChild("IdleAnimation"));
    if (m_pIdleAnimation != nullptr) {
        m_pIdleAnimation->SetRGBA(0,0,0,0);
    }
    m_pJumpAnimation = dynamic_cast<AnimatedSpriteNode *>(GetChild("JumpAnimation"));
    if (m_pJumpAnimation != nullptr) {
        m_pJumpAnimation->SetRGBA(0,0,0,0);
    }
    m_pWalkingAnimation = dynamic_cast<AnimatedSpriteNode *>(GetChild("WalkAnimation"));
    if (m_pWalkingAnimation != nullptr) {
        m_pWalkingAnimation->SetRGBA(0,0,0,0);
    }
    m_pGroundSensor = dynamic_cast<ColliderNode *>(GetChild("GroundSensor"));
    if (m_pGroundSensor != nullptr) {
        m_pGroundSensor->OnEntry.Register<Player>(&Player::OnLandOnGround, *this);
    }
    m_pWallSensorLeft = dynamic_cast<ColliderNode *>(GetChild("WallSensorLeft"));
    if (m_pWallSensorLeft != nullptr) {
        m_pWallSensorLeft->OnEntry.Register<Player>(&Player::OnHitWallLeft, *this);
    }
    m_pWallSensorRight = dynamic_cast<ColliderNode *>(GetChild("WallSensorRight"));
    if (m_pWallSensorRight != nullptr) {
        m_pWallSensorRight->OnEntry.Register<Player>(&Player::OnHitWallRight, *this);
    }
    m_pLevelGoal = dynamic_cast<ColliderNode*>(GetChild("GoalSensor"));
    if (m_pLevelGoal != nullptr) {
        m_pLevelGoal->OnEntry.Register<Player>(&Player::HandleCollision, *this);
    }
    m_pHookSprite = dynamic_cast<SpriteNode*>(GetChild("HookSprite"));
}


void Player::SetupParameter(IniParser *parser, const std::string &section) {

    Node::SetupParameter(parser, section); // IMORTANT!!

    // own setup of variables
    m_fGroundAcceleration = parser->GetValueAsFloat(section, "groundAcceleration");
    m_fGroundDeceleration = parser->GetValueAsFloat(section, "groundDeceleration");
    m_fGroundMaxSpeed = parser->GetValueAsFloat(section, "maxGroundSpeed");
    m_fJumpForce = parser->GetValueAsFloat(section, "jumpForce");
    m_iMaxJumps = parser->GetValueAsInt(section, "maxJumps");
    m_fMinRange = parser->GetValueAsFloat(section, "minHookRange");
    m_fMaxRange = parser->GetValueAsFloat(section, "maxHookRange");
    m_fDamping = parser->GetValueAsFloat(section, "damping");
    m_fHertz = parser->GetValueAsFloat(section, "hertz");
    m_fSwingAcceleration = parser->GetValueAsFloat(section, "swingAcceleration");
    m_fSwingDeceleration = parser->GetValueAsFloat(section, "swingDeceleration");
    m_bJointSpringEnabled = parser->GetValueAsBoolean(section, "hasSpring");

    // child setup
    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/whoosh/Player.ini");
}



void Player::Process(float deltaTime) {
    Node::Process(deltaTime);
    if (m_pRigidBody == nullptr)
        return;

    if (m_bCheatsEnabled) {
        HandleMovementCheat(deltaTime);
        return;
    }

    HandleMovement(deltaTime);
    HandleHookControls();
    HandleHookVelocity();
    HandleHookVisualisation();
    HandleAnimations();
    HandleFlip();
    HandleSound();

    m_bIsGrounded = false;
}

void Player::HandleMovementCheat(float deltaTime) const {

    auto pos = m_pRigidBody->GetBodyPosition();
    float flySpeed = 500 * deltaTime;
    if (InputManager::GetInstance().GetButtonState(SDLK_d) == BS_HELD) {
        pos.x += flySpeed;
        m_pRigidBody->SetPositionInMeters(PhysicsManager::PixelsToMeterVector(pos));
    }

    if (InputManager::GetInstance().GetButtonState(SDLK_a) == BS_HELD) {
        pos.x -= flySpeed;
        m_pRigidBody->SetPositionInMeters(PhysicsManager::PixelsToMeterVector(pos));
    }

    if (InputManager::GetInstance().GetButtonState(SDLK_w) == BS_HELD) {
        pos.y -= flySpeed;
        m_pRigidBody->SetPositionInMeters(PhysicsManager::PixelsToMeterVector(pos));
    }

    if (InputManager::GetInstance().GetButtonState(SDLK_s) == BS_HELD) {
        pos.y += flySpeed;
        m_pRigidBody->SetPositionInMeters(PhysicsManager::PixelsToMeterVector(pos));
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
        if (m_iJumpsMade < m_iMaxJumps) {
            m_pRigidBody->ResetBodyVelocity();
            m_pRigidBody->ApplyImpluseToCenter(Vector2d(0,-m_fJumpForce));
            auto jumpSound = SoundManager::GetInstance().PlaySound("playerJump.wav");
            jumpSound->setVolume(7);
            m_iJumpsMade++;
        }
    }

    if (m_bIsHittingWall) {
        m_velocity.x = 0;
        m_bIsHittingWall = false;
    }

    if (!m_bIsHookAttached) {
        m_pRigidBody->SetHorizontalVelocity(m_velocity);
    }
}

void Player::HandleAnimations() {
    float walkingThreshold = m_fGroundMaxSpeed * 0.7f;

    if (!m_bIsGrounded && !m_pJumpAnimation->m_bIsAnimating) {
        ChangeAnimation(m_pJumpAnimation);
        m_pCurrentAnimation->SetLooping(false);
        m_pLandingSound = nullptr;
    }

    if (m_bIsGrounded) {
        if (abs(m_velocity.x) == 0) {
            ChangeAnimation(m_pIdleAnimation);
        } else if (abs(m_velocity.x) < walkingThreshold) {
            ChangeAnimation(m_pWalkingAnimation);
        } else {
            ChangeAnimation(m_pRunningAnimation);
        }
    }
}

void Player::HandleSound() {
    float walkingThreshold = m_fGroundMaxSpeed * 0.7f;

    if (abs(m_velocity.x) > walkingThreshold && m_bIsGrounded) {
        if (m_pWalkingSound == nullptr) {
            m_pWalkingSound = SoundManager::GetInstance().PlaySound("playerStep.wav");
            m_pWalkingSound->setMode(FMOD_LOOP_NORMAL);
            m_pWalkingSound->setVolume(5);
            m_pWalkingSound->setPitch(2.5);
        }
    }
    else {
        m_pWalkingSound->stop();
        m_pWalkingSound = nullptr;
    }
}

void Player::DestroyHook() {
    if (b2Joint_IsValid(m_b2Hook)) {
        m_globalHookOrigin = Vector2d::Zero();
        m_globalHookTarget = Vector2d::Zero();
        b2DestroyJoint(m_b2Hook, false);
    }
}

void Player::HandleFlip() const {
    if (m_pCurrentAnimation == nullptr) return;

    if (m_pCurrentAnimation->IsFlipped() && !m_bIsFlipped) {
        m_pCurrentAnimation->Flip();
    }
    if (!m_pCurrentAnimation->IsFlipped() && m_bIsFlipped) {
        m_pCurrentAnimation->Flip();
    }
}

void Player::HandleHookControls() {

    auto currentMouseButtonEvent = InputManager::GetInstance().GetCurrentMouseEvent();
    Vector2d mousePosition = InputManager::GetInstance().GetMousePosition();



    // filter out duplicates of the same event
    if (m_lastMouseButtonEvent.type == currentMouseButtonEvent.type) return;
    m_lastMouseButtonEvent = currentMouseButtonEvent;

    // only receive left mouse button UP events
    if (currentMouseButtonEvent.button != 1 || currentMouseButtonEvent.type != SDL_MOUSEBUTTONUP) return;

    // check hook states
    if (m_bIsHookAttached) {
        DestroyHook();

        m_bIsHookAttached = false;
    }
    else {
        auto hasShotHook = ShootHookSwing(mousePosition);
        if (hasShotHook) {
            m_bIsHookAttached = true;
        }
    }
}

void Player::HandleHookVelocity() const {
    if (!m_bIsHookAttached) return;
    if (!b2Joint_IsValid(m_b2Hook)) return;

    auto currentVelocity = m_pRigidBody->GetBodyVelocity();

    // accelerate while falling (y velocity > 0) and direction is pressed
    if (m_pRigidBody->GetBodyVelocity().y > 0) {
        if (m_pRigidBody->GetBodyVelocity().x > 0 && InputManager::GetInstance().GetButtonState(SDLK_d) == BS_HELD) {
            m_pRigidBody->SetHorizontalVelocity( Vector2d(currentVelocity.x * m_fSwingAcceleration, currentVelocity.y ));

        }
        else if (m_pRigidBody->GetBodyVelocity().x > 0) {
            m_pRigidBody->SetHorizontalVelocity( Vector2d(currentVelocity.x * m_fSwingDeceleration, currentVelocity.y ));

        }
        if (m_pRigidBody->GetBodyVelocity().x < 0 && InputManager::GetInstance().GetButtonState(SDLK_a) == BS_HELD) {
            m_pRigidBody->SetHorizontalVelocity( Vector2d(currentVelocity.x * m_fSwingAcceleration, currentVelocity.y ));
        }
        else if (m_pRigidBody->GetBodyVelocity().x < 0) {
            m_pRigidBody->SetHorizontalVelocity( Vector2d(currentVelocity.x * m_fSwingDeceleration, currentVelocity.y ));
        }
    }
}

void Player::HandleHookVisualisation() const {
    if (m_pHookSprite == nullptr) return;
    if (m_globalHookOrigin == Vector2d::Zero() || m_globalHookTarget == Vector2d::Zero()) {
        m_pHookSprite->SetRGBA(0,0,0,0);
        return;
    }

    auto hookOffset = Vector2d{80,80};
    auto currentOrigin = GetGlobalPosition() + hookOffset;

    // length of hook
    auto distance = currentOrigin.Distance(m_globalHookTarget);
    m_pHookSprite->SetBaseSize(Vector2d(distance*2, 2));

    // angle of hook
    auto direction = m_globalHookTarget - currentOrigin;
    auto angle = atan2(direction.y, direction.x) * 180/std::numbers::pi;

    m_pHookSprite->SetRGBA(1,1,1,1);
    auto newOffsetPos = Vector2d{currentOrigin} - Vector2d{distance, 0};
    m_pHookSprite->SetGlobalPosition(newOffsetPos);
    m_pHookSprite->m_globalTransform.SetRotation(-angle);

}

void Player::Reset(Vector2d pos) {
    m_bHasTargetObjectReceived = false;
    m_bIsHookAttached = false;
    m_bCheatsEnabled = false;
    m_bHasLevelCompleted = false;
    DestroyHook();
    SetGlobalPosition(pos);
}

bool Player::ShootHookSwing(Vector2d posInPixel) {

    Vector2d hookOffset = Vector2d{80,80};

    b2WorldId world = PhysicsManager::GetInstance().GetWorld();
    Vector2d playerPositionInBox2DWorld = PhysicsManager::PixelsToMeterVector(GetGlobalPosition() + hookOffset);
    Vector2d targetPositionInBox2DWorld = PhysicsManager::PixelsToMeterVector(posInPixel);

    b2RayResult rayResult = CastRayFromTo(playerPositionInBox2DWorld, targetPositionInBox2DWorld, world);

    if (!rayResult.hit) {
        m_globalHookOrigin = Vector2d::Zero();
        m_globalHookTarget = Vector2d::Zero();
        return false;
    };

    // body that got hit
    b2BodyId targetBodyId = b2Shape_GetBody(rayResult.shapeId);

    CreateChainBetween(playerPositionInBox2DWorld,rayResult.point, targetBodyId, world);

    m_globalHookOrigin = GetGlobalPosition() + hookOffset;
    m_globalHookTarget = PhysicsManager::MeterToPixelsVector(Vector2d{rayResult.point.x, rayResult.point.y});

    SoundManager::GetInstance().PlaySound("grapplingHook.mp3");

    return true;
}


void Player::CreateChainBetween(b2Vec2 origin, b2Vec2 target, b2BodyId targetBody, b2WorldId world) {

    // local point of raycast source
    b2Transform localTransformPlayer;
    localTransformPlayer.p = b2Body_GetLocalPoint(m_pRigidBody->GetBodyId(), origin);
    localTransformPlayer.q = b2MakeRot(0);

    // transform of point hit
    b2Transform localTransformTarget;
    localTransformTarget.p = b2Body_GetLocalPoint(targetBody,target);
    localTransformTarget.q = b2MakeRot(0);


    b2JointDef playerJointDef = b2JointDef();
    playerJointDef.bodyIdA = m_pRigidBody->GetBodyId();
    playerJointDef.bodyIdB = targetBody;
    playerJointDef.localFrameA = localTransformPlayer;
    playerJointDef.localFrameB = localTransformTarget;
    playerJointDef.collideConnected = true;

    b2DistanceJointDef distanceJointDef = b2DefaultDistanceJointDef();
    distanceJointDef.base = playerJointDef;
    distanceJointDef.enableSpring = m_bJointSpringEnabled;
    distanceJointDef.enableLimit = true;
    distanceJointDef.hertz = m_fHertz;
    distanceJointDef.dampingRatio = m_fDamping;
    distanceJointDef.minLength = m_fMinRange;
    distanceJointDef.maxLength = m_fMaxRange;

    auto distanceJoint = b2CreateDistanceJoint(PhysicsManager::GetInstance().GetWorld(), &distanceJointDef);
    m_b2Hook = distanceJoint;
    b2Joint_WakeBodies(m_b2Hook);
}


void Player::HandleCollision(const b2ShapeId* collidedShapes) {
    if (collidedShapes == nullptr) return;
    if (!b2Shape_IsValid(collidedShapes[0])) return;
    if (!b2Shape_IsSensor(collidedShapes[0])) return;

    auto collisionData = b2Body_GetUserData(b2Shape_GetBody(collidedShapes[0]));
    auto collisionDataNode = static_cast<ColliderNode*>(collisionData);

    if (collisionDataNode != nullptr && collisionDataNode->m_name == "ObjectPickup" && !m_bHasTargetObjectReceived) {
        m_bHasTargetObjectReceived = true;
    };
    if (collisionDataNode != nullptr && collisionDataNode->m_name == "FinishArea" && m_bHasTargetObjectReceived) {
        SceneManager::GetInstance().SetSceneActive("WinScreen");
        auto teleportSound = SoundManager::GetInstance().PlaySound("teleport.wav");
        teleportSound->setVolume(4);

    };

    if (collisionDataNode != nullptr && collisionDataNode->m_name == "HurtArea") {
        Vector2d pos;
        pos.x = m_iniParser->GetValueAsFloat(m_UId, "globalPosX");
        pos.y = m_iniParser->GetValueAsFloat(m_UId, "globalPosY");
        Reset(pos);
    };
}



void Player::ChangeAnimation(AnimatedSpriteNode* animation) {
    if (animation == nullptr) return;

    if (m_pCurrentAnimation == nullptr) {
        m_pCurrentAnimation = animation;
    } else {
        m_pCurrentAnimation->SetRGBA(0, 0, 0, 0);
        m_pCurrentAnimation = animation;
    }

    m_pCurrentAnimation->SetRGBA(1, 1, 1, 1);
    m_pCurrentAnimation->SetAnimating(true);
    m_pCurrentAnimation->SetLooping(true);
}


// ############# EVENT CALLBACKS ################

void Player::OnHitWallRight(const b2ShapeId* target) {
    if (target == nullptr) return;
    if (!b2Shape_IsValid(target[0])) return;
    if (b2Shape_IsSensor(target[0])) return;
    if (b2Body_GetType(b2Shape_GetBody(target[0])) != b2_staticBody) return;

    if (m_velocity.x > 0) {
        m_bIsHittingWall = true;
    }
}

void Player::OnHitWallLeft(const b2ShapeId* target) {
    if (target == nullptr) return;
    if (!b2Shape_IsValid(target[0])) return;
    if (b2Shape_IsSensor(target[0])) return;
    if (b2Body_GetType(b2Shape_GetBody(target[0])) != b2_staticBody) return;

    if (m_velocity.x < 0) {
        m_bIsHittingWall = true;
    }

}

void Player::OnLandOnGround(const b2ShapeId* target) {
    if (target == nullptr) return;
    if (!b2Shape_IsValid(target[0])) return;
    if (b2Shape_IsSensor(target[0])) return;
    if (b2Body_GetType(b2Shape_GetBody(target[0])) != b2_staticBody) return;

    m_bIsGrounded = true;
    m_iJumpsMade = 0;

    if (m_pLandingSound == nullptr) {
        m_pLandingSound = SoundManager::GetInstance().PlaySound("playerLand.wav");
        m_pLandingSound->setVolume(5);
    }
}



// ######## HOOK RAY CASTING ###############

float RayCastCallback(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void *context) {

    auto attachedBody = b2Shape_GetBody(shapeId);
    auto rayResult = static_cast<b2RayResult*>(context);
    if (rayResult == nullptr) return -1;

    if (b2Body_GetType(attachedBody) != b2_staticBody) {
        return 1;
    }
    if (b2Shape_IsSensor(shapeId) == true) {
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
    const b2Vec2 translation = (target - origin) * 10;

    // Set up query filter
    const b2QueryFilter filter = b2DefaultQueryFilter();

    auto* rayResult = new b2RayResult();

    // Cast ray and get closest hit
    b2World_CastRay(world, origin, translation, filter, RayCastCallback, rayResult);

    return *rayResult;
}

// ############# GETTER ###############

float Player::GetHookMinRange() const {
    return m_fMinRange;
}

float Player::GetHookMaxRange() const {
    return m_fMaxRange;
}

float Player::GetHookDamping() const {
    return m_fDamping;
}

float Player::GetHookHertz() const {
    return m_fHertz;
}

float Player::GetHookSwingAcceleration() const {
    return m_fSwingAcceleration;
}
float Player::GetHookSwingDeceleration() const {
    return m_fSwingDeceleration;
}

bool Player::GetHookSpringEnabled() const {
    return m_bJointSpringEnabled;
}


// ############### SETTER ###################
void Player::SetHookMinRange(float min) {
    m_fMinRange = min;
}
void Player::SetHookMaxRange(float max) {
    m_fMaxRange = max;
}

void Player::SetHookDamping(float damping) {
    m_fDamping = damping;
}

void Player::SetHookHertz(float hertz) {
    m_fHertz = hertz;
}

void Player::SetHookSwingAcceleration(float acceleration) {
    m_fSwingAcceleration = acceleration;
}

void Player::SetHookSwingDeceleration(float deceleration) {
    m_fSwingDeceleration = deceleration;
}

void Player::SetHookSpringEnabled(bool enabled) {
    m_bJointSpringEnabled = enabled;
}
