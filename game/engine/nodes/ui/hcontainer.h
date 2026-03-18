#ifndef GP_FRAMEWORK_HCONTAINER_H
#define GP_FRAMEWORK_HCONTAINER_H

#include "container.h"
namespace Engine {
    class HContainer : public Container {
    public:
        HContainer();
        ~HContainer() override;

        void PositionChildren() override;
        void CalculateChildSpace() const override;

    };
}




#endif //GP_FRAMEWORK_HCONTAINER_H