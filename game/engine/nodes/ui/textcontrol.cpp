#include "textcontrol.h"

#include "imgui.h"
#include "../nodefactory.h"
#include "../../logmanager/logmanager.h"

namespace Engine {
    TextControl::TextControl() : m_bIsInitialised(false), m_iPointSize(16), m_textSprite(nullptr), m_rgba{0,0,0,255} {


        SetupNode("TextControl", NT_TextControl);

        m_nodeInfo.push_back(
            {
                "", [](Node &n) {
                    ImGui::Separator();
                    ImGui::Text("Text Settings");
                }
            });
        m_nodeInfo.push_back({
                "Text", [](Node &n) {
                    if (auto *s = dynamic_cast<TextControl *>(&n)) {
                        auto currentText = s->GetText();
                        if (ImGui::InputText("##Editor", currentText, 28)) {
                            s->SetText(std::string(currentText));
                            s->SetValue("text", currentText);
                        }
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "Layer", [](Node &n) {
                    if (auto *s = dynamic_cast<TextControl *>(&n)) {
                        if (s->m_textSprite != nullptr) {
                            ImGui::DragInt("Layer", &s->m_textSprite->m_iLayer, 1, 0, 10);
                        }
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "PointSize", [](Node &n) {
                    if (auto *s = dynamic_cast<TextControl *>(&n)) {
                        if (s->m_textSprite != nullptr) {
                            if (ImGui::DragInt("Layer", &s->m_iPointSize, 1, 0, 10)) {
                                s->SetValue("pointSize", s->m_iPointSize);
                                s->m_bIsInitialised = false;
                            }
                        }
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "TextColor", [](Node &n) {
                    if (auto *s = dynamic_cast<TextControl *>(&n)) {
                        if (ImGui::ColorPicker4("Layer", s->m_rgba, 1)) {
                            s->m_bIsInitialised = false;
                            s->SetValue("redTint", s->m_rgba[2]);
                            s->SetValue("greenTint", s->m_rgba[1]);
                            s->SetValue("blueTint", s->m_rgba[0]);
                            s->SetValue("alpha", s->m_rgba[3]);
                        }
                    }
                }
            }
        );
    }


    void TextControl::Init() {
        Control::Init();
    }

    void TextControl::Draw(Renderer& renderer) {
        Control::Draw(renderer);
        if (!m_bIsInitialised && !m_text.empty()) {
            // Load static text textures into the Texture Manager...
            auto color = SDL_Color(static_cast<Uint8>(m_rgba[2]*255), static_cast<Uint8>(m_rgba[1]*255),static_cast<Uint8>(m_rgba[0]*255),static_cast<Uint8>(m_rgba[3]));
            renderer.CreateStaticText(m_text.c_str(), m_iPointSize, color);
            // Generate sprites that use the static text textures...
            m_textSprite = renderer.CreateSprite(m_text.c_str());
            m_textSprite->SetX(m_globalTransform.position.x);
            m_textSprite->SetY(m_globalTransform.position.y);
        }

        if (m_textSprite != nullptr) {
            m_textSprite->SetX(m_globalTransform.position.x);
            m_textSprite->SetY(m_globalTransform.position.y);
            m_textSprite->Draw(renderer);
            m_bIsInitialised = true;
        }
    }

    void TextControl::SystemProcess() {
        Control::SystemProcess();

        if (m_textSprite != nullptr) {
            m_initialSize.x = m_textSprite->GetWidth();
            m_initialSize.y = m_textSprite->GetHeight();
            m_textSprite->SetX(m_globalTransform.position.x + m_textSprite->GetWidth()/2);
            m_textSprite->SetY(m_globalTransform.position.y + m_textSprite->GetHeight()/2);
            m_textSprite->SetAngle(m_globalTransform.GetRotationDeg());
        }
    }

    void TextControl::SetupParameter(IniParser *parser, const std::string &sectionId) {
        Control::SetupParameter(parser, sectionId);
        m_text = parser->GetValueAsString(sectionId, "text");
        m_rgba[2] = parser->GetValueAsFloat(sectionId, "redTint");
        m_rgba[1] = parser->GetValueAsFloat(sectionId, "greenTint");
        m_rgba[0] = parser->GetValueAsFloat(sectionId, "blueTint");
        m_rgba[3] = parser->GetValueAsFloat(sectionId, "alpha");
        m_iPointSize = parser->GetValueAsFloat(sectionId, "pointSize");
    }

    void TextControl::SetText(std::string str) {
        m_text = str;
        m_bIsInitialised = false;
    }

    void TextControl::SetTextRGBA(float r, float g, float b, float a) {
        m_rgba[2] = r;
        m_rgba[1] = g;
        m_rgba[0] = b;
        m_rgba[3] = a;
        m_bIsInitialised = false;
    }

    const float* TextControl::GetTextRGBA() const {
        return m_rgba;
    }

    void TextControl::SetPointSize(int pointSize) {
        m_iPointSize = pointSize;
        m_bIsInitialised = false;
    }

    char * TextControl::GetText() {
        return m_text.data();
    }
}
