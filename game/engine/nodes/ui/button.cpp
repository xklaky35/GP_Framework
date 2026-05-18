#include "button.h"
#include "../nodefactory.h"
#include "../../input/input.h"

Button::Button() : m_pTemplateButton(nullptr) {
    SetupNode("Button", NT_Button);

    OnClick = Event<Button>();

}
void Button::Init() {
    TextControl::Init();
    m_pTemplateButton = new SpriteNode();
    m_pTemplateButton->SetSpritePath("../assets/Sprites/rect.png");
    m_pTemplateButton->m_spriteDisplayMode = Fit;
    m_pTemplateButton->SetBaseSize(Vector2d{m_globalTransform.GetWidth(), m_globalTransform.GetHeight()});
    AddChild(*m_pTemplateButton);
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
