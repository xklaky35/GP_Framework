#include "player.h"

#include "imgui.h"
#include "../../engine/input/input.h"
#include "../../engine/nodes/nodefactory.h"

using namespace Engine;

Player::Player() : m_rigidBody(nullptr), m_bIsGrounded(true), m_bIsFlipped(false),
                   m_fGroundAcceleration(0), m_fGroundDeceleration(0), m_fGroundMinSpeed(0),
                   m_fGroundMaxSpeed(0), m_jumpsMade(0), m_maxJumps(1), m_fJumpForce(0), m_currentAnimation(nullptr) {


    REGISTER_CLASS(Player);
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

