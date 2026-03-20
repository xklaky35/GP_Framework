#ifndef GP_FRAMEWORK_MARGINCONTAINER_H
#define GP_FRAMEWORK_MARGINCONTAINER_H
#include "container.h"


namespace Engine {
    class MarginContainer : public Container {
    public:
        MarginContainer();
        void Init() override;
        void SystemProcess();
        void PositionChildren() override;
        void CalculateChildSpace() const override;

    public:
        float m_MarginLeft;
        float m_MarginRight;
        float m_MarginTop;
        float m_MarginBottom;

    };
}

#endif //GP_FRAMEWORK_MARGINCONTAINER_H