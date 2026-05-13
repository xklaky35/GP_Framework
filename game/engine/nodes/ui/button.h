#ifndef GP_FRAMEWORK_BUTTON_H
#define GP_FRAMEWORK_BUTTON_H
#include "control.h"
#include "textcontrol.h"
#include "../../event.h"

using namespace Engine;

class Button : public TextControl {

public:

    Button();
    void Init() override;
    void Draw(Renderer &) override;
    void Process(float deltaTime) override;
    void SetupParameter(IniParser *parser, const std::string &sectionId) override;

public:
    Event<Button> OnClick;

private:
    SpriteNode* m_templateButton;

};


#endif //GP_FRAMEWORK_BUTTON_H