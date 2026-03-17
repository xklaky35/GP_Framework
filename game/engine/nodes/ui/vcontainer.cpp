#include "vcontainer.h"

namespace Engine {
    VContainer::VContainer() = default;
    VContainer::~VContainer() = default;


    void VContainer::Init() {
        Control::Init();
    }

    void VContainer::Process(float deltaTime) {
        Control::Process(deltaTime);
        PositionChildren();
        CalculateChildBounds();
    }

    void VContainer::SystemProcess() {
        Control::SystemProcess();
    }

    void VContainer::Draw(Renderer & renderer) {
        Control::Draw(renderer);
    }

    void VContainer::PositionChildren() {
        std::vector<Control*> childSections;

        // get child sections
        for (Node* c : children) {
            if (auto* childControl = dynamic_cast<Control*>(c)) {
                if (childControl->m_containerSizing.m_bExpandVertical) {
                    childSections.push_back(childControl);
                }
                else {
                    childControl->m_controlSpace.y = m_initialSize.y;
                }
                if (childControl->m_containerSizing.m_bExpandHorizontal) {
                    childControl->m_controlSpace.x = m_globalTransform.GetWidth();
                    childControl->m_globalTransform.position.x = m_globalTransform.position.x;
                }
                else {
                    childControl->m_controlSpace.x = m_initialSize.x;
                }
            }
        }

        float childOffset = 0;
        childOffset = m_globalTransform.GetHeight() / childSections.size();

        for (int i = 0; i < childSections.size(); i++) {
            childSections[i]->m_globalTransform.position.y = childOffset * i;
            childSections[i]->m_controlSpace.y = childOffset;
        }
    }

    void VContainer::CalculateChildBounds() {
        // get child sections
        for (Node* c : children) {
            if (auto* childControl = dynamic_cast<Control*>(c)) {
                // horizontal bounds
                switch (childControl->m_containerSizing.m_horizontalBehavior) {
                    case h_Fill:
                        childControl->m_globalTransform.SetWidth(childControl->m_controlSpace.x);
                        break;
                    case h_Left:
                        childControl->m_globalTransform.SetWidth(childControl->m_initialSize.x);
                        childControl->m_globalTransform.position.x = m_globalTransform.position.x;
                        break;
                    case h_Center:
                        childControl->m_globalTransform.SetWidth(childControl->m_initialSize.x);
                        childControl->m_globalTransform.position.x = (m_globalTransform.position.x + childControl->m_controlSpace.x / 2) - childControl->m_globalTransform.GetWidth() / 2;
                        break;
                    case h_Right:
                        childControl->m_globalTransform.SetWidth(childControl->m_initialSize.x);
                        childControl->m_globalTransform.position.x = m_globalTransform.position.x + childControl->m_controlSpace.x - childControl->m_globalTransform.GetWidth();
                        break;
                }
                switch (childControl->m_containerSizing.m_verticalBehavior) {
                    case v_Fill:
                        childControl->m_globalTransform.SetHeight(childControl->m_controlSpace.y);
                        break;
                    case v_Top:
                        childControl->m_globalTransform.SetHeight(childControl->m_initialSize.y);
                        break;
                    case v_Center:
                        childControl->m_globalTransform.SetHeight(childControl->m_initialSize.y);
                        childControl->m_globalTransform.position.y = (childControl->m_globalTransform.position.y + childControl->m_controlSpace.y / 2) - childControl->m_globalTransform.GetHeight() / 2;
                        break;
                    case v_Bottom:
                        childControl->m_globalTransform.SetHeight(childControl->m_initialSize.y);
                        childControl->m_globalTransform.position.y = childControl->m_globalTransform.position.y + childControl->m_controlSpace.y - childControl->m_globalTransform.GetHeight();
                        break;
                }
            }
        }
    }
}
