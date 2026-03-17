#ifndef GP_FRAMEWORK_MAINMEU_H
#define GP_FRAMEWORK_MAINMEU_H

#include "../../engine/nodes/ui/vcontainer.h"
#include "../../engine/nodes/ui/control.h"

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

};


#endif //GP_FRAMEWORK_MAINMEU_H