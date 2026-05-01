#include "spritenode.h"

#include <numeric>
#include <utility>

#include "imgui.h"
#include "nodefactory.h"
#include "../../helper/inlinehelper.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Engine {
    /**
     * This node uses the sprites dimensions by default.
     */
    SpriteNode::SpriteNode() : m_iLayer(0), m_spriteDisplayMode(Original),
                               m_pSprite(nullptr),
                               m_pRenderer(nullptr),
                               m_redTint(1),
                               m_greenTint(1), m_blueTint(1), m_alpha(1), m_scaleFactor(0) {

        SetupNode("SpriteNode", NT_SpriteNode);
        m_spritePath = {};
        m_nodeInfo.push_back(
            {
                "", [](Node &n) {
                    ImGui::Separator();
                    ImGui::Text("Sprite Settings");
                }
            });
        m_nodeInfo.push_back({
                "SpritePath", [](Node &n) {
                    if (auto *s = dynamic_cast<SpriteNode *>(&n)) {
                        if (ImGui::InputText("##Editor", &s->m_spritePath)) { // only works by including "misc/cpp/imgui_stdlib.cpp"
                            s->SetValue("spritePath", s->m_spritePath);
                        }
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "RedTint", [](Node &n) {
                    if (auto *s = dynamic_cast<SpriteNode *>(&n)) {
                        int v_min = 0, v_max = 1;
                        ImGui::SetNextItemWidth(-FLT_MIN);
                        if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_redTint, 1, 0.5f, &v_min,
                                               &v_max)) {
                            s->SetValue("redTint", s->m_redTint);
                        }
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "GreenTint", [](Node &n) {
                    if (auto *s = dynamic_cast<SpriteNode *>(&n)) {
                        int v_min = 0, v_max = 1;
                        ImGui::SetNextItemWidth(-FLT_MIN);
                        if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_greenTint, 1, 0.5f, &v_min,
                                               &v_max)) {
                            s->SetValue("greenTint", s->m_greenTint);
                        }
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "BlueTint", [](Node &n) {
                    if (auto *s = dynamic_cast<SpriteNode *>(&n)) {
                        int v_min = 0, v_max = 1;
                        ImGui::SetNextItemWidth(-FLT_MIN);
                        if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_blueTint, 1, 0.5f, &v_min,
                                               &v_max)) {
                            s->SetValue("blueTint", s->m_blueTint);
                        }
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "Alpha", [](Node &n) {
                    if (auto *s = dynamic_cast<SpriteNode *>(&n)) {
                        int v_min = 0, v_max = 1;
                        ImGui::SetNextItemWidth(-FLT_MIN);
                        if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_alpha, 1, 0.5f, &v_min, &v_max)) {
                            s->SetValue("alpha", s->m_alpha);
                        }
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "Layer", [](Node &n) {
                    if (auto *s = dynamic_cast<SpriteNode *>(&n)) {
                        if (s->m_pSprite == nullptr) return;
                        if (ImGui::DragInt("Layer", &s->m_pSprite->m_iLayer, 1, 0, 10)) {
                            s->SetValue("layer", s->m_pSprite->m_iLayer);
                        }
                    }
                }
            }
        );
        m_nodeInfo.push_back(
            {
                "Sprite Display Mode", [](Node &n) {
                    if (auto *s = dynamic_cast<SpriteNode *>(&n)) {
                        if (ImGui::BeginCombo("Horizontal:", "Select container behavior")) {
                            for (int i = 0; i < 3; i++) {
                                bool is_selected = s->m_spriteDisplayMode == static_cast<SpriteDisplayFlag>(i);
                                if (ImGui::Selectable(SpriteDisplayFlagStrings[i], is_selected)) {
                                    s->m_spriteDisplayMode = static_cast<SpriteDisplayFlag>(i);
                                    s->SetValue("spriteDisplayMode",SpriteDisplayFlagStrings[i]);
                                }
                            }
                            ImGui::EndCombo();
                        }
                    }
                }
            });
    }

    void SpriteNode::Init() {
        Node::Init();
    }


    void SpriteNode::SystemProcess() {
        Node::SystemProcess();
        if (m_pSprite != nullptr) {
            switch (m_spriteDisplayMode) {
                case Original:
                    m_pSprite->SetWidth(m_pSprite->GetOriginalWidth());
                    m_pSprite->SetHeight(m_pSprite->GetOriginalHeight());
                    break;
                case Fit:
                    m_pSprite->SetWidth(m_globalTransform.GetWidth());
                    m_pSprite->SetHeight(m_globalTransform.GetHeight());
                    break;
                case Scale:
                    // only the hight can be configured to scale the sprite
                    if (m_pSprite->GetWidth() != 0 && m_pSprite->GetHeight() != 0) {
                        m_aspectRatio.x = std::round(m_pSprite->GetOriginalWidth() / m_pSprite->GetOriginalHeight() * 100) / 100;
                        m_aspectRatio.y = 1;
                    }

                    m_pSprite->SetWidth(m_globalTransform.GetHeight() * m_aspectRatio.x);
                    m_pSprite->SetHeight(m_globalTransform.GetHeight());
                    break;
            }
            m_pSprite->SetX(m_globalTransform.position.x + m_pSprite->GetWidth() / 2);
            m_pSprite->SetY(m_globalTransform.position.y + m_pSprite->GetHeight() / 2);
            m_pSprite->SetAngle(m_globalTransform.GetRotationDeg());
        }
    }

    void SpriteNode::Draw(Renderer &renderer) {
        Node::Draw(renderer);

        SetupSpriteRendering(renderer);

        if (m_pSprite != nullptr) {
            m_pSprite->SetAlpha(m_alpha);
            m_pSprite->SetRedTint(m_redTint);
            m_pSprite->SetGreenTint(m_greenTint);
            m_pSprite->SetBlueTint(m_blueTint);
            m_pSprite->SetScale(m_globalTransform.GetScale());
            m_pSprite->SetAngle(m_globalTransform.GetRotationDeg());
            m_pSprite->Draw(renderer);
        }
    }

    void SpriteNode::DrawDebug() {
        Node::DrawDebug();
    }


    void SpriteNode::SetupSpriteRendering(Renderer &renderer) {
        if (m_pRenderer == nullptr) {
            m_pRenderer = &renderer;
        }

        if (!m_spritePath.empty() && m_pSprite == nullptr && m_pRenderer != nullptr) {
            m_pSprite = m_pRenderer->CreateSprite(m_spritePath.c_str());
            m_pSprite->m_iLayer = m_iLayer;
        }
    }


    /**
     * All parameters take values between 0 and 1. Highter/lower values will be set to max 1 and min 0 by the renderer
     * @param r red tint
     * @param g green tint
     * @param b blue tint
     * @param a alpha value
     */
    void SpriteNode::SetRGBA(const float r, const float g, const float b, const float a) {
        m_redTint = r;
        m_greenTint = g;
        m_blueTint = b;
        m_alpha = a;
    }

    void SpriteNode::SetSpritePath(std::string path) {
        m_spritePath = std::move(path);
    }

    void SpriteNode::SetupParameter(IniParser *parser, const std::string &section) {
        Node::SetupParameter(parser, section);
        m_spriteDisplayMode = static_cast<SpriteDisplayFlag>(GetIndexOf(
        SpriteDisplayFlagStrings, parser->GetValueAsString(section, "spriteDisplayMode").c_str(), SPRITE_DISPLAY_FLAG_STRING_COUNT));
        m_iLayer = parser->GetValueAsInt(section, "layer");
        SetRGBA(
            parser->GetValueAsFloat(section, "redTint"),
            parser->GetValueAsFloat(section, "blueTint"),
            parser->GetValueAsFloat(section, "greenTint"),
            parser->GetValueAsFloat(section, "alpha")
        );
        SetSpritePath(parser->GetValueAsString(section, "spritePath"));
    }


}
