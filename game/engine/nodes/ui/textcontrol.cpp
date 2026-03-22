#include "textcontrol.h"

#include "../../logmanager/logmanager.h"

namespace Engine {
    TextControl::TextControl() : m_bIsInitialised(false), m_iPointSize(16), m_textSprite(nullptr), m_rgba{0,0,0,255} {
        m_nodeInfo.push_back(
            {
                "", [](Node &n) {
                    ImGui::Separator();
                    ImGui::Text("Text Settings");
                }
            });
        m_nodeInfo.push_back({
                "Text", [](Node &n) {
                    if (TextControl *s = dynamic_cast<TextControl *>(&n)) {
                        if (ImGui::InputText("##Editor", const_cast<char *>(s->m_text.c_str()), 28)) {
                            s->m_bIsInitialised = false;
                        }
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "Layer", [](Node &n) {
                    if (TextControl *s = dynamic_cast<TextControl *>(&n)) {
                        ImGui::DragInt("Layer", &s->m_textSprite->m_iLayer, 1, 0, 10);
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "Layer", [](Node &n) {
                    if (TextControl *s = dynamic_cast<TextControl *>(&n)) {
                        if (ImGui::ColorPicker4("Layer", s->m_rgba, 1)) {
                            s->m_bIsInitialised = false;
                        }
                    }
                }
            }
        );
    }


    void TextControl::Init() {
        Control::Init();
        m_name = "Textcontrol";
    }

    void TextControl::Draw(Renderer& renderer) {
        Control::Draw(renderer);
        if (!m_bIsInitialised && !m_text.empty()) {
            // Load static text textures into the Texture Manager...
            auto color = SDL_Color(static_cast<Uint8>(m_rgba[2]*255), static_cast<Uint8>(m_rgba[1]*255),static_cast<Uint8>(m_rgba[0]*255),static_cast<Uint8>(m_rgba[3]));
            renderer.CreateStaticText(m_text.c_str(), 50, color);
            // Generate sprites that use the static text textures...
            m_textSprite = renderer.CreateSprite(m_text.c_str());
            m_bIsInitialised = true;
        }
        m_textSprite->Draw(renderer);
    }

    void TextControl::SystemProcess() {
        Control::SystemProcess();

        if (m_textSprite != nullptr) {
            m_initialSize.x = m_textSprite->GetWidth();
            m_initialSize.y = m_textSprite->GetHeight();
            m_textSprite->SetX(m_globalTransform.position.x + m_textSprite->GetWidth() / 2);
            m_textSprite->SetY(m_globalTransform.position.y + m_textSprite->GetHeight() / 2);
            m_textSprite->SetAngle(m_globalTransform.GetRotationDeg());
        }
    }

    void TextControl::SetText(std::string str) {
        m_text = str;
        m_bIsInitialised = false;
    }
}
