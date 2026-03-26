#include "panelcontainer.h"

#include "imgui.h"

namespace Engine {
    PanelContainer::PanelContainer() : m_background(nullptr), m_transparency(0.4) {

        m_nodeInfo.push_back(
            {
                "", [](Node &n) {
                    ImGui::Separator();
                    ImGui::Text("Panel Settings");
                }
            });
        m_nodeInfo.push_back({
                "Transparency", [](Node &n) {
                    if (auto *s = dynamic_cast<PanelContainer *>(&n)) {
                        float v_min = 0, v_max = 1;
                        ImGui::SetNextItemWidth(-FLT_MIN);
                        ImGui::DragFloat("##Editor",  &s->m_transparency, 0.5f, v_min, v_max);
                    }
                }
            }
        );
    }


    void PanelContainer::Init() {
        Container::Init();
        m_name = "PanelContainer";

        m_background = new SpriteNode();
        m_background->SetSpritePath(std::string("../assets/Sprites/rect.png").data());
        m_background->m_spriteDisplayMode = Fit;
        m_background->SetRGBA(1,1,1,m_transparency);
        AddChild(*m_background);
    }

    void PanelContainer::PositionChildren() {
        for (int i = 0; i < m_children.size(); i++) {
            if (auto* childControl = dynamic_cast<Control*>(m_children[i])) {

            }
        }
    }

    void PanelContainer::CalculateChildSpace() const {
        for (int i = 0; i < m_children.size(); i++) {
            if (auto *childControl = dynamic_cast<Control *>(m_children[i])) {
                if (childControl->m_containerSizing.m_bExpandVertical) {
                    childControl->m_controlSpace.y = m_controlSpace.y;
                } else {
                    childControl->m_controlSpace.y = m_initialSize.y;
                }
                if (childControl->m_containerSizing.m_bExpandHorizontal) {
                    childControl->m_controlSpace.x = m_controlSpace.x;
                } else {
                    childControl->m_controlSpace.x = m_initialSize.x;
                }
            }
        }
    }
}
