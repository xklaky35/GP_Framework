#include "container.h"

namespace Engine {

    void Container::SystemProcess() {
        Control::SystemProcess();
        CalculateInitialSize();
        CalculateChildSpace();
        CalculateChildBounds();
        PositionChildren();
    }

    void Container::CalculateInitialSize() {
        float maxChildWidth = 0;
        float maxChildHeight = 0;
        for (int i = 0; i < m_children.size(); i++) {
            if (auto* childControl = dynamic_cast<Control*>(m_children[i])) {
                if (childControl->m_initialSize.x > maxChildWidth) maxChildWidth = childControl->m_initialSize.x;
                if (childControl->m_initialSize.y > maxChildHeight) maxChildHeight = childControl->m_initialSize.y;
            }
        }
        m_initialSize.x = maxChildWidth;
        m_initialSize.y = maxChildHeight;
    }

}