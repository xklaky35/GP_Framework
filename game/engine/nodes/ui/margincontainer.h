#ifndef GP_FRAMEWORK_MARGINCONTAINER_H
#define GP_FRAMEWORK_MARGINCONTAINER_H
#include "container.h"


namespace Engine {
    class MarginContainer : public Container {

        void PositionChildren() override;
        void CalculateChildSpace() const override;


    };
}

#endif //GP_FRAMEWORK_MARGINCONTAINER_H