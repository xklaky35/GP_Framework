#ifndef GP_FRAMEWORK_MARGINCONTAINER_H
#define GP_FRAMEWORK_MARGINCONTAINER_H
#include "container.h"


namespace Engine {
    class MarginContainer : public Container {
    public:
        MarginContainer();
        void Init() override;
        void PositionChildren() override;
        void SystemProcess() override;
        void CalculateChildSpace() const override;

    public:
        float m_fMarginLeft;
        float m_fMarginRight;
        float m_fMarginTop;
        float m_fMarginBottom;

    };
}

#endif //GP_FRAMEWORK_MARGINCONTAINER_H