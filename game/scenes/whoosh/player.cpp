
#include "player.h"

#include "imgui.h"
#include "../../engine/input/input.h"
#include "../../engine/nodes/nodefactory.h"


using namespace Engine;

Player::Player() : m_rigidBody(nullptr), m_bIsGrounded(true), m_bIsFlipped(false),
                   m_fGroundAcceleration(0), m_fGroundDeceleration(0), m_fGroundMinSpeed(0),
                   m_fGroundMaxSpeed(0), m_jumpsMade(0), m_maxJumps(1) {
    m_name = "Player";
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
                        s->m_iniParser->SetValue(s->GetUId(), "groundAcceleration", s->m_fGroundAcceleration, s);
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
                        s->m_iniParser->SetValue(s->GetUId(), "groundDeceleration", s->m_fGroundDeceleration, s);
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
                        s->m_iniParser->SetValue(s->GetUId(), "maxGroundSpeed", s->m_fGroundMaxSpeed, s);
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
                        s->m_iniParser->SetValue(s->GetUId(), "maxJumps", s->m_maxJumps, s);
                    }
                }
            }
        }
    );
    m_nodeInfo.push_back({
            "Velocity X", [](Node &n) {
                if (auto *s = dynamic_cast<Player *>(&n)) {
                    int v_min = -100000, v_max = 100000;
                    ImGui::SetNextItemWidth(-FLT_MIN);

                    ImGui::BeginDisabled();
                    ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_velocity.x, 1, 0.5f, &v_min, &v_max);
                    ImGui::EndDisabled();
                }
            }
        }
    );
    m_nodeInfo.push_back({
            "Velocity Y", [](Node &n) {
                if (auto *s = dynamic_cast<Player *>(&n)) {
                    int v_min = -100000, v_max = 100000;
                    ImGui::SetNextItemWidth(-FLT_MIN);

                    ImGui::BeginDisabled();
                    ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_velocity.y, 1, 0.5f, &v_min, &v_max);
                    ImGui::EndDisabled();
                }
            }
        }
    );
}


void Player::Init() {
    Node::Init();

    // child setup
    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/whoosh/player.ini");

    // own setup of variables
    m_fGroundAcceleration = m_iniParser->GetValueAsFloat(GetUId(), "groundAcceleration");
    m_fGroundDeceleration = m_iniParser->GetValueAsFloat(GetUId(), "groundDeceleration");
    m_fGroundMaxSpeed = m_iniParser->GetValueAsFloat(GetUId(), "maxGroundSpeed");
    m_maxJumps = m_iniParser->GetValueAsInt(GetUId(), "maxJumps");


    m_rigidBody = new RigidbodyNode(b2_dynamicBody, 0, 0);
    m_rigidBody->m_globalTransform.SetSize(m_globalTransform.GetWidth(), m_globalTransform.GetHeight());
    AddChild(*m_rigidBody);


}

void Player::Process(float deltaTime) {
    Node::Process(deltaTime);
    HandleMovement(deltaTime);
}


void Player::HandleMovement(float deltaTime) {

    if (InputManager::GetInstance().GetButtonState(SDLK_d) == BS_HELD) {

        if (m_velocity.x < m_fGroundMaxSpeed) {
            m_moveDirection.x = 1;
            m_velocity += m_moveDirection * m_fGroundAcceleration * deltaTime;
        }
    }
    else {
        if (m_velocity.x > 0) {
            m_velocity.x -= m_fGroundDeceleration * deltaTime;
        }
    }

    if (InputManager::GetInstance().GetButtonState(SDLK_a) == BS_HELD) {
        if (abs(m_velocity.x) < m_fGroundMaxSpeed) {
            m_moveDirection.x = -1;
            m_velocity += m_moveDirection * m_fGroundAcceleration * deltaTime;
        }
    }
    else {
        if (m_velocity.x < 0) {
            m_velocity.x += m_fGroundDeceleration * deltaTime;
        }
    }

    if (InputManager::GetInstance().GetButtonState(SDLK_SPACE) == BS_PRESSED) {
        if (m_jumpsMade < m_maxJumps) {
            m_rigidBody->ResetBody();
            m_rigidBody->ApplyImpluseToCenter(Vector2d(0,-150));
            m_jumpsMade++;
        }
    }

    m_rigidBody->SetVelocity(m_velocity);
    m_globalTransform.position = m_rigidBody->GetBodyPosition();
}


void Player::OnLandOnGround() {
    m_bIsGrounded = true;
    m_jumpsMade = 0;
}


