#include "margincontainer.h"

#include "imgui.h"
#include "../nodefactory.h"

namespace Engine {
    MarginContainer::MarginContainer()
        : m_fMarginLeft(0),
          m_fMarginRight(0),
          m_fMarginTop(0),
          m_fMarginBottom(0) {

        SetupNode("MarginContainer", NT_MarginContainer);
        m_nodeInfo.push_back(
            {
                "", [](Node &n) {
                    ImGui::Separator();
                    ImGui::Text("MarginContainer Settings");
                }
            });
        m_nodeInfo.push_back(

            {
                "MarginLeft", [](Node &n) {
                    if (auto c = dynamic_cast<MarginContainer *>(&n)) {
                        int v_min = -10000, v_max = 10000;
                        ImGui::SetNextItemWidth(-FLT_MIN);
                        ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &c->m_fMarginLeft, 1, 0.5f,
                                           &v_min, &v_max);
                    }

                }
            });
        m_nodeInfo.push_back(

            {
                "MarginRight", [](Node &n) {
                    if (auto c = dynamic_cast<MarginContainer *>(&n)) {
                        int v_min = -10000, v_max = 10000;
                        ImGui::SetNextItemWidth(-FLT_MIN);
                        ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &c->m_fMarginRight, 1, 0.5f,
                                           &v_min, &v_max);
                    }

                }
            });
        m_nodeInfo.push_back(

            {
                "MarginTop", [](Node &n) {
                    if (auto *c = dynamic_cast<MarginContainer *>(&n)) {
                        int v_min = -10000, v_max = 10000;
                        ImGui::SetNextItemWidth(-FLT_MIN);
                        ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &c->m_fMarginTop, 1, 0.5f,
                                           &v_min, &v_max);
                    }

                }
            });
        m_nodeInfo.push_back(

            {
                "MarginBottom", [](Node &n) {
                    if (auto *c = dynamic_cast<MarginContainer *>(&n)) {
                        int v_min = -10000, v_max = 10000;
                        ImGui::SetNextItemWidth(-FLT_MIN);
                        ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &c->m_fMarginBottom, 1, 0.5f,
                                           &v_min, &v_max);
                    }

                }
            });
    }

    void MarginContainer::Init() {
        Container::Init();
        m_name = "MarginContainer";
    }

    void MarginContainer::SystemProcess() {
        if (m_fMarginRight < 0) m_fMarginRight = 0;
        if (m_fMarginLeft < 0) m_fMarginLeft = 0;
        if (m_fMarginTop < 0) m_fMarginTop = 0;
        if (m_fMarginBottom < 0) m_fMarginBottom = 0;

        Container::SystemProcess();

    }


    void MarginContainer::PositionChildren() {
        assert(m_children.size() == 1);
        if (auto* childControl = dynamic_cast<Control*>(m_children[0])) {
             childControl->m_transform.position.x = m_transform.position.x + m_fMarginLeft;
            childControl->m_transform.position.y = m_transform.position.y + m_fMarginTop;
        }
    }

    void MarginContainer::CalculateChildSpace() const {
        assert(m_children.size() == 1);
        if (auto* childControl = dynamic_cast<Control*>(m_children[0])) {
            if (childControl->m_containerSizing.m_bExpandVertical) {
                childControl->m_controlSpace.y = m_controlSpace.y - m_fMarginTop - m_fMarginBottom;
            }
            else {
                childControl->m_controlSpace.y = m_initialSize.y;
            }
            if (childControl->m_containerSizing.m_bExpandHorizontal) {
                childControl->m_controlSpace.x = m_controlSpace.x - m_fMarginRight - m_fMarginLeft;
            }
            else {
                childControl->m_controlSpace.x = m_initialSize.x;
            }
        }

    }
}
