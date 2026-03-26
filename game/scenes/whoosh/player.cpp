
#include "player.h"

#include "imgui.h"
#include "../../engine/input/input.h"
#include "../../engine/nodes/nodefactory.h"

using namespace Engine;

Player::Player() : m_spriteNode(nullptr), m_rigidBody(nullptr), m_bIsGrounded(true), m_fGroundAcceleration(200),
                   m_fGroundDeceleration(400),
                   m_fGroundMaxSpeed(400), m_jumpsMade(0), m_maxJumps(1) {
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
                        if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_fGroundAcceleration, 1, 0.5f, &v_min, &v_max)) {
                            s->m_iniParser->SetValue("Player", "groundAcceleration", s->m_fGroundAcceleration);
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
                        s->m_iniParser->SetValue("Player", "groundDeceleration", s->m_fGroundDeceleration);
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
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_fGroundMaxSpeed, 1, 0.5f, &v_min, &v_max)) {
                        s->m_iniParser->SetValue("Player", "maxGroundSpeed", s->m_fGroundMaxSpeed);
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
                        s->m_iniParser->SetValue("Player", "maxJumps", s->m_maxJumps);
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

Player::~Player() {
    m_iniParser->SaveChangesToIniFile();
    delete m_iniParser;
    m_iniParser = nullptr;
};

void Player::Init() {
    Node::Init();

    m_iniParser = new IniParser();
    m_iniParser->LoadIniFile("../game/scenes/whoosh/player.ini");

    // own setup of variables
    m_fGroundAcceleration = m_iniParser->GetValueAsFloat("Player", "groundAcceleration");
    m_fGroundDeceleration = m_iniParser->GetValueAsFloat("Player", "groundDeceleration");
    m_fGroundMaxSpeed = m_iniParser->GetValueAsFloat("Player", "maxGroundSpeed");
    m_maxJumps = m_iniParser->GetValueAsInt("Player", "maxJumps");

    // child setup
    NodeFactory::GetInstance().AttachChildren(this);

    m_rigidBody = new RigidbodyNode(b2_dynamicBody);
    AddChild(*m_rigidBody);

}

void Player::Process(float deltaTime) {
    Node::Process(deltaTime);
    HandleMovement(deltaTime);
}


void Player::HandleMovement(float deltaTime) {

    if (InputManager::GetCurrentEvents().GetButtonState(SDLK_d)) {
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

    if (InputManager::GetCurrentEvents().GetButtonState(SDLK_a)) {
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

    if (InputManager::GetCurrentEvents().GetButtonState(SDLK_SPACE)) {
        if (m_jumpsMade < m_maxJumps) {
            m_rigidBody->AddForceToCenter(Vector2d(0,500));
            //m_jumpsMade++;
        }
    }

    //m_deltaSum += deltaTime;
    m_globalTransform.position.x += m_velocity.x * deltaTime;
    m_globalTransform.position.y += m_velocity.y * deltaTime;
}


void Player::OnLandOnGround() {
    m_bIsGrounded = true;
    m_jumpsMade = 0;
}


