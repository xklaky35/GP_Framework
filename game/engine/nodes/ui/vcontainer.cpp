#include "vcontainer.h"

namespace Engine {
    VContainer::VContainer() = default;
    VContainer::~VContainer() = default;

    void VContainer::Init() {
        Container::Init();
        m_name = "VContainer";
    }


    void VContainer::PositionChildren() {
        float offset = 0;
        for (int i = 0; i < m_children.size(); i++) {
            if (auto* childControl = dynamic_cast<Control*>(m_children[i])) {
                childControl->m_transform.position.y = offset;
                offset += childControl->m_controlSpace.y;
            }
        }
    }

    void VContainer::CalculateChildSpace() const {
        std::vector<Control*> childSections;
        float usedSpace = 0;
        // get child sections
        for (Node* c : m_children) {
            if (auto* childControl = dynamic_cast<Control*>(c)) {
                if (childControl->m_containerSizing.m_bExpandVertical) {
                    childSections.push_back(childControl);
                }
                else {
                    childControl->m_controlSpace.y = m_initialSize.y;
                    usedSpace += childControl->m_controlSpace.y;
                }
                if (childControl->m_containerSizing.m_bExpandHorizontal) {
                    childControl->m_controlSpace.x = m_globalTransform.GetWidth();
                }
                else {
                    childControl->m_controlSpace.x = m_initialSize.x;
                }
            }
        }

        if (childSections.size() > 0) {
            float  sectionSize = (m_globalTransform.GetHeight() - usedSpace) / childSections.size();
            for (Control* c : childSections) {
                c->m_controlSpace.y = sectionSize;
            }
        }

    }
}
