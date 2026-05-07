#ifndef GP_FRAMEWORK_MAINMEU_H
#define GP_FRAMEWORK_MAINMEU_H

#include "../../engine/nodes/nodefactory.h"
#include "../../engine/nodes/ui/vcontainer.h"
#include "../../engine/nodes/ui/hcontainer.h"
#include "../../engine/nodes/ui/control.h"
#include "../../engine/nodes/ui/textcontrol.h"

using namespace Engine;

class MainMenu : public HContainer {

public:
    MainMenu();

    void Init() override;
    void Draw(Renderer& renderer) override;

    void HandleMouseInteraction();

    void ExecuteOption();

    void Process(float) override;
    void SetupParameter(IniParser *parser, const std::string &sectionId) override;

    VContainer* vcontainer1;
    HContainer* h1;
    HContainer* h2;
    HContainer* h3;
    HContainer* h4;
    TextControl* t1;
    TextControl* t2;
    Control* spacer1;
    Control* spacer2;
    Control* spacer3;
    Control* spacer4;
    TextControl* cursor;
    bool m_bSelectionLocked;
    int m_currentSelection;
    HContainer* m_selectionParents[4];
    std::string m_sceneOptions[4];
};
REGISTER_CLASS(MainMenu)

#endif //GP_FRAMEWORK_MAINMEU_H