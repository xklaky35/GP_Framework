#include "hcontainer.h"


namespace Engine {

    HContainer::HContainer() = default;
    HContainer::~HContainer() = default;

    void HContainer::Init() {
        Container::Init();
        m_name = "HContainer";
    }


    void HContainer::CalculateChildSpace() const {
        std::vector<Control*> childSections;
        float usedSpace = 0;
        // get child sections
        for (Node* c : m_children) {
            if (auto* childControl = dynamic_cast<Control*>(c)) {
                if (childControl->m_containerSizing.m_bExpandHorizontal) {
                    childSections.push_back(childControl);
                }
                else {
                    childControl->m_controlSpace.x = childControl->m_initialSize.x;
                    usedSpace += childControl->m_controlSpace.x;
                }
                if (childControl->m_containerSizing.m_bExpandVertical) {
                    childControl->m_controlSpace.y = m_controlSpace.y;
                }
                else {
                    childControl->m_controlSpace.y = childControl->m_initialSize.y;
                }
            }
        }

        if (childSections.size() > 0) {
            float  sectionSize = (m_controlSpace.x - usedSpace) / childSections.size();
            for (Control* c : childSections) {
                c->m_controlSpace.x = sectionSize;
            }
        }

    }

    void HContainer::PositionChildren() {
        float offset = 0;
        for (int i = 0; i < m_children.size(); i++) {
            if (auto* childControl = dynamic_cast<Control*>(m_children[i])) {
                childControl->m_transform.position.x += offset;
                offset += childControl->m_controlSpace.x;
            }
        }
    }

    void HContainer::CalculateInitialSize() {
        float maxChildHeight = 0;
        float width = 0;
        for (int i = 0; i < m_children.size(); i++) {
            if (auto *childControl = dynamic_cast<Control *>(m_children[i])) {
                width += childControl->m_initialSize.x;
                if (childControl->m_initialSize.y > maxChildHeight) maxChildHeight = childControl->m_initialSize.y;
            }
        }
        m_initialSize.y = maxChildHeight;
        m_initialSize.x = width;
    }
}