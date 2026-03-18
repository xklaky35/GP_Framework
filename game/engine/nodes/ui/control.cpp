#include "control.h"
#include "../../../helper/inlinehelper.h"

namespace Engine {
    Control::Control() : Control(false) {}
    Control::Control(bool useDebugRect) : m_bUseDebugRect(useDebugRect), m_positionMode(Position), m_bIsChildOfContainer(false), m_debugRect(nullptr) {
        m_containerSizing = ContainerSizing();


        m_nodeInfo.push_back(
            {
                "", [](Node &n) {
                    ImGui::Separator();
                    ImGui::Text("Control Settings");
                }
            });
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
        m_nodeInfo.push_back(

            {
                "ControlSpaceX", [](Node &n) {
                    if (Control *c = dynamic_cast<Control *>(&n)) {
                        int v_min = -10000, v_max = 10000;
                        ImGui::SetNextItemWidth(-FLT_MIN);
                        ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &c->m_controlSpace.x, 1, 0.5f,
                                           &v_min, &v_max);
                    }

                }
            });
        m_nodeInfo.push_back(

            {
                "ControlSpaceY", [](Node &n) {
                    if (Control *c = dynamic_cast<Control *>(&n)) {
                        int v_min = -10000, v_max = 10000;
                        ImGui::SetNextItemWidth(-FLT_MIN);
                        ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &c->m_controlSpace.y, 1, 0.5f,
                                           &v_min, &v_max);
                    }

                }
            });
    }

    Control::~Control() = default;

    void Control::Init() {
        Node::Init();
        m_name = "Control";
        m_globalTransformationFlag = Disable;

        if (m_bUseDebugRect) {
            m_debugRect = new SpriteNode("../assets/Sprites/board8x8.png");
            m_debugRect->m_spriteDisplayMode = Fit;
            m_debugRect->SetRGBA(GetRandomPercentage(), GetRandomPercentage(),GetRandomPercentage(),1);
            AddChild(*m_debugRect);
        }

        m_globalTransform.SetSize(50,50);
        m_initialSize = Vector2d(m_globalTransform.GetWidth(), m_globalTransform.GetHeight());
        m_controlSpace = Vector2d(m_globalTransform.GetWidth(), m_globalTransform.GetHeight());
    }

    void Control::Process(float x) {
        Node::Process(x);
    }

    void Control::SystemProcess() {
        Node::SystemProcess();
        if (m_parent != nullptr) {
            m_globalTransform.position = m_parent->m_globalTransform.position + m_transform.position;
        }
    }

    void Control::Draw(Renderer &renderer) {
        Node::Draw(renderer);
    }

    void Control::CalculateChildBounds() const {
        // get child sections
        for (Node* c : m_children) {
            if (auto* childControl = dynamic_cast<Control*>(c)) {
                // horizontal bounds
                switch (childControl->m_containerSizing.m_horizontalBehavior) {
                    case h_Fill:
                        childControl->m_globalTransform.SetWidth(childControl->m_controlSpace.x);
                        break;
                    case h_Left:
                        childControl->m_globalTransform.SetWidth(childControl->m_initialSize.x);
                        childControl->m_transform.position.x = 0;
                        break;
                    case h_Center:
                        childControl->m_globalTransform.SetWidth(childControl->m_initialSize.x);
                        childControl->m_transform.position.x = (childControl->m_controlSpace.x / 2) - childControl->m_globalTransform.GetWidth() / 2;
                        break;
                    case h_Right:
                        childControl->m_globalTransform.SetWidth(childControl->m_initialSize.x);
                        childControl->m_transform.position.x = childControl->m_controlSpace.x - childControl->m_globalTransform.GetWidth();
                        break;
                }
                switch (childControl->m_containerSizing.m_verticalBehavior) {
                    case v_Fill:
                        childControl->m_globalTransform.SetHeight(childControl->m_controlSpace.y);
                        break;
                    case v_Top:
                        childControl->m_globalTransform.SetHeight(childControl->m_initialSize.y);
                        childControl->m_transform.position.y = 0;
                        break;
                    case v_Center:
                        childControl->m_globalTransform.SetHeight(childControl->m_initialSize.y);
                        childControl->m_transform.position.y = (childControl->m_transform.position.y + childControl->m_controlSpace.y / 2) - childControl->m_globalTransform.GetHeight() / 2;
                        break;
                    case v_Bottom:
                        childControl->m_globalTransform.SetHeight(childControl->m_initialSize.y);
                        childControl->m_transform.position.y = childControl->m_controlSpace.y - childControl->m_globalTransform.GetHeight();
                        break;
                }
            }
        }
    }

}
