#ifndef GP_FRAMEWORK_CENTERCONTAINER_H
#define GP_FRAMEWORK_CENTERCONTAINER_H

#include "container.h"
namespace Engine {
    class PanelContainer : public Container {
    public:

        PanelContainer();
        ~PanelContainer() override;
        void Init() override;
        void PositionChildren() override;
        void CalculateChildSpace() const override;


    public:
        SpriteNode* m_background;
        float m_transparency;

    };
}

#endif //GP_FRAMEWORK_CENTERCONTAINER_H