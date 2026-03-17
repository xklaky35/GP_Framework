#include "control.h"

namespace Engine {
    Control::Control() : Control(false) {}
    Control::Control(bool useDebugRect) : m_bUseDebugRect(useDebugRect), m_positionMode(Position), m_bIsChildOfContainer(false), m_debugRect(nullptr) {
        m_containerSizing = ContainerSizing();


        m_nodeInfo.push_back(
            {
                "Horizontal Behavior", [](Node &n) {
                    if (Control* c = dynamic_cast<Control*>(&n)) {
                        if (ImGui::BeginCombo("Horizontal:", "Select container behavior")) {
                            for (int i = 0; i <= 3; i++) {
                                bool is_selected =
                                    c->m_containerSizing.m_horizontalBehavior == static_cast<ContainerBehaviorHorizontal>(i);
                                if (ImGui::Selectable(ContainerBehaviorHorizontalStrings[i], is_selected)) {
                                    c->m_containerSizing.m_horizontalBehavior = static_cast<ContainerBehaviorHorizontal>(i);
                                }
                            }
                            ImGui::EndCombo();
                        }
                    }
                }
            });

        m_nodeInfo.push_back(
            {
                "Vertical Behavior", [](Node &n) {
                    if (Control *c = dynamic_cast<Control *>(&n)) {
                        if (ImGui::BeginCombo("Vertical:", "Select container behavior")) {
                            for (int i = 0; i <= 3; i++) {
                                bool is_selected =
                                        c->m_containerSizing.m_verticalBehavior == static_cast<ContainerBehaviorVertical>(i);
                                if (ImGui::Selectable(ContainerBehaviorVerticalStrings[i], is_selected)) {
                                    c->m_containerSizing.m_verticalBehavior = static_cast<ContainerBehaviorVertical>(i);
                                }
                            }
                            ImGui::EndCombo();
                        }
                    }
                }
            });

        m_nodeInfo.push_back(
            {
                "Expand Horizontal", [](Node &n) {
                    if (Control *c = dynamic_cast<Control *>(&n)) {
                        ImGui::Checkbox("Expand", &c->m_containerSizing.m_bExpandHorizontal);
                    }
                }
            });

        m_nodeInfo.push_back(
            {
                "Expand Vertical", [](Node &n) {
                    if (Control *c = dynamic_cast<Control *>(&n)) {
                        ImGui::Checkbox("Expand", &c->m_containerSizing.m_bExpandVertical);
                    }
                }
            });
    }

    Control::~Control() = default;

    void Control::Init() {
        Node::Init();
        m_globalTransformationFlag = Disable;

        if (m_bUseDebugRect) {
            m_debugRect = new SpriteNode("../assets/Sprites/rect.png");
            m_debugRect->m_spriteDisplayMode = Fit;
            AddChild(*m_debugRect);
        }


        m_globalTransform.SetSize(50,50);
        m_initialSize = Vector2d(m_globalTransform.GetWidth(), m_globalTransform.GetHeight());
    }

    void Control::Process(float x) {
        Node::Process(x);
    }

    void Control::SystemProcess() {
        Node::SystemProcess();
    }

    void Control::Draw(Renderer &renderer) {
        Node::Draw(renderer);
    }
}
