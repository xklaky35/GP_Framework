#ifndef GP_FRAMEWORK_CONTAINER_H
#define GP_FRAMEWORK_CONTAINER_H

#include "../../nodes/ui/control.h"

namespace Engine {
    class Container : public Control {
    public:
        void SystemProcess() override;
        virtual void PositionChildren() = 0;
        virtual void CalculateChildSpace() const = 0;
    };

    inline void Container::SystemProcess() {
        Control::SystemProcess();
        CalculateChildSpace();
        PositionChildren();
        CalculateChildBounds();
    }
}

#endif //GP_FRAMEWORK_CONTAINER_H