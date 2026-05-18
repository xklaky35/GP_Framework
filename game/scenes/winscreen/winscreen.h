#ifndef GP_FRAMEWORK_WINSCREEN_H
#define GP_FRAMEWORK_WINSCREEN_H
#include "../../engine/nodes/node.h"
#include "../../engine/nodes/nodefactory.h"
#include "../../engine/nodes/ui/hcontainer.h"
#include "../../engine/nodes/ui/textcontrol.h"
#include "../../engine/nodes/ui/vcontainer.h"

using namespace Engine;

class WinScreen : public HContainer {
public:
    WinScreen();
    void Init() override;
    void Draw(Renderer& renderer) override;
    void HandleMouseInteraction();
    void ExecuteSelection() const;

    void Process(float) override;

    VContainer* vcontainer1;
    HContainer* h1;
    HContainer* h2;
    HContainer* h3;
    HContainer* h4;
    TextControl* t1;
    TextControl* t2;
    TextControl* t3;
    Control* spacer1;
    Control* spacer2;
    Control* spacer3;
    Control* spacer4;
    int m_iCurrentSelection;

private:
    std::string m_completionTime;
};
REGISTER_CLASS(WinScreen)


#endif //GP_FRAMEWORK_WINSCREEN_H