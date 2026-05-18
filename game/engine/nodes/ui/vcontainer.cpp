#include "vcontainer.h"

#include "../nodefactory.h"

namespace Engine {
    VContainer::VContainer() = default;

    void VContainer::Init() {
        Container::Init();
        SetupNode("VContainer", NT_VContainer);
    }

    void VContainer::PositionChildren() {
        float offset = 0;
        for (auto & child : m_children) {
            if (auto* childControl = dynamic_cast<Control*>(child)) {
                childControl->m_transform.position.y += offset;
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
                    childControl->m_controlSpace.y = childControl->m_initialSize.y;
                    usedSpace += childControl->m_controlSpace.y;
                }
                if (childControl->m_containerSizing.m_bExpandHorizontal) {
                    childControl->m_controlSpace.x = m_controlSpace.x;
                }
                else {
                    childControl->m_controlSpace.x = childControl->m_initialSize.x;
                }
            }
        }

        if (!childSections.empty()) {
            float  sectionSize = (m_controlSpace.y - usedSpace) / static_cast<float>(childSections.size());
            for (Control* c : childSections) {
                c->m_controlSpace.y = sectionSize;
            }
        }

    }

    void VContainer::CalculateInitialSize() {
        float maxChildWidth = 0;
        float height = 0;
        for (auto & child : m_children) {
            if (auto *childControl = dynamic_cast<Control *>(child)) {
                height += childControl->m_initialSize.y;
                if (childControl->m_initialSize.x > maxChildWidth) maxChildWidth = childControl->m_initialSize.x;
            }
        }
        m_initialSize.x = maxChildWidth;
        m_initialSize.y = height;
    }
}
