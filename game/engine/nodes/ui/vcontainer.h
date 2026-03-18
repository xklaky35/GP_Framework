#ifndef GP_FRAMEWORK_VCONTAINER_H
#define GP_FRAMEWORK_VCONTAINER_H

#include "container.h"
namespace Engine {

    class VContainer : public Container {
    public:
        VContainer();
        ~VContainer() override;

        void PositionChildren() override;
        void CalculateChildSpace() const override;
    };
}

#endif //GP_FRAMEWORK_VCONTAINER_H