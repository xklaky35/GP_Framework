#ifndef GP_FRAMEWORK_MAINMEU_H
#define GP_FRAMEWORK_MAINMEU_H

#include "../../engine/nodes/ui/vcontainer.h"
#include "../../engine/nodes/ui/hcontainer.h"
#include "../../engine/nodes/ui/control.h"
#include "../../engine/nodes/ui/textcontrol.h"

using namespace Engine;

class MainMenu : public HContainer {

public:
    MainMenu();
    ~MainMenu() override;

    void Init() override;
    void Draw(Renderer& renderer) override;
    void Process(float) override;

    VContainer vcontainer1;
    HContainer h1;
    HContainer h2;
    HContainer h3;
    HContainer h4;
    TextControl t1;
    TextControl t2;
    TextControl t3;
    TextControl t4;
    Control spacer;
    TextControl cursor;
    bool m_bSelectionLocked;
    int m_currentSelection;
    HContainer* m_selectionParents[4];
    std::string m_sceneOptions[4];
};


#endif //GP_FRAMEWORK_MAINMEU_H