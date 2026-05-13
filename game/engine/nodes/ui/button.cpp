#include "button.h"
#include "../nodefactory.h"
#include "../../input/input.h"

Button::Button() {
    SetupNode("Button", NT_Button);

    OnClick = Event<Button>();

}
void Button::Init() {
    TextControl::Init();
    m_templateButton = new SpriteNode();
    m_templateButton->SetSpritePath("../assets/Sprites/rect.png");
    m_templateButton->m_spriteDisplayMode = Fit;
    m_templateButton->SetBaseSize(Vector2d{m_globalTransform.GetWidth(), m_globalTransform.GetHeight()});
    AddChild(*m_templateButton);
}

void Button::Draw(Renderer &renderer) {
    TextControl::Draw(renderer);

}

void Button::Process(float deltaTime) {

    TextControl::Process(deltaTime);


}
void Button::SetupParameter(IniParser *parser, const std::string &sectionId) {
    TextControl::SetupParameter(parser, sectionId);

}
