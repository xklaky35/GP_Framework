#ifndef GP_FRAMEWORK_CONTAINER_H
#define GP_FRAMEWORK_CONTAINER_H

#include "../../nodes/ui/control.h"

namespace Engine {
    class Container : public Control {
    public:
        virtual void PositionChildren() = 0;
        virtual void CalculateChildBounds() = 0;
    };
}

#endif //GP_FRAMEWORK_CONTAINER_H