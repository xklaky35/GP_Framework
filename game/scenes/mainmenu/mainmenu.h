#ifndef GP_FRAMEWORK_MAINMEU_H
#define GP_FRAMEWORK_MAINMEU_H

#include "../../engine/nodes/ui/vcontainer.h"
#include "../../engine/nodes/ui/hcontainer.h"
#include "../../engine/nodes/ui/control.h"
#include "../../engine/nodes/ui/margincontainer.h"
#include "../../engine/nodes/ui/panelcontainer.h"

using namespace Engine;

class MainMenu : public VContainer {

public:
    MainMenu();
    ~MainMenu() override;

    void Init() override;
    void Draw(Renderer& renderer) override;

    Control testBox1;
    Control testBox2;
    Control testBox3;
    Control testBox4;
    Control testBox5;
    VContainer vcontainer1;
    HContainer hcontainer2;
    MarginContainer mcontainer1;
    PanelContainer panelcontainer1;

};


#endif //GP_FRAMEWORK_MAINMEU_H