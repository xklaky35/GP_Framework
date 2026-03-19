#include "spritenode.h"

#include <numeric>

namespace Engine {
    /**
     * This node uses the sprites dimensions by default.
     */
    SpriteNode::SpriteNode() : Node("Sprite"),
                               m_spriteDisplayMode(Original), m_pSpritePath(nullptr),
                               m_pSprite(nullptr),
                               m_pRenderer(nullptr),
                               m_scaleFactor(0),
                               m_redTint(1), m_greenTint(1), m_blueTint(1), m_alpha(1) {

        m_nodeInfo.push_back(
            {
                "", [](Node &n) {
                    ImGui::Separator();
                    ImGui::Text("Sprite Settings");
                }
            });
        m_nodeInfo.push_back({
                "SpritePath", [](Node &n) {
                    if (SpriteNode* s = dynamic_cast<SpriteNode*>(&n))
                        ImGui::InputText("##Editor",  const_cast<char*>(s->m_pSpritePath), 28);
                }
            }
        );
        m_nodeInfo.push_back({
                "RedTint", [](Node &n) {
                    if (SpriteNode *s = dynamic_cast<SpriteNode *>(&n)) {
                        int v_min = 0, v_max = 1;
                        ImGui::SetNextItemWidth(-FLT_MIN);
                        ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_redTint, 1, 0.5f, &v_min, &v_max);
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "GreenTint", [](Node &n) {
                    if (SpriteNode *s = dynamic_cast<SpriteNode *>(&n)) {
                        int v_min = 0, v_max = 1;
                        ImGui::SetNextItemWidth(-FLT_MIN);
                        ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_greenTint, 1, 0.5f, &v_min, &v_max);
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "BlueTint", [](Node &n) {
                    if (SpriteNode *s = dynamic_cast<SpriteNode *>(&n)) {
                        int v_min = 0, v_max = 1;
                        ImGui::SetNextItemWidth(-FLT_MIN);
                        ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_blueTint, 1, 0.5f, &v_min, &v_max);
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "Alpha", [](Node &n) {
                    if (SpriteNode *s = dynamic_cast<SpriteNode *>(&n)) {
                        int v_min = 0, v_max = 1;
                        ImGui::SetNextItemWidth(-FLT_MIN);
                        ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_alpha, 1, 0.5f, &v_min, &v_max);
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "Layer", [](Node &n) {
                    if (SpriteNode *s = dynamic_cast<SpriteNode *>(&n)) {
                        ImGui::DragInt("Layer", &s->m_pSprite->m_iLayer, 1, 0, 10);
                    }
                }
            }
        );
    }

    /**
     * This node uses the sprites dimensions by default. If you specify a custom height and width, please disable m_bUseSpriteSize.
     * @param spritePath path to the sprite image
     */
    SpriteNode::SpriteNode(const char *spritePath) : SpriteNode() {
        m_pSpritePath = spritePath;
    }

    /**
     * This node uses the sprites dimensions by default. If you specify a custom height and width, please disable m_bUseSpriteSize.
     * @param height hight the sprite should be rendered with
     * @param width width the sprite should be rendered with
     * @param spritePath path to the sprite image
     */
    SpriteNode::SpriteNode(float height, float width, const char *spritePath) : SpriteNode(spritePath){
        m_globalTransform.SetHeight(height);
        m_globalTransform.SetWidth(width);
    }

    SpriteNode::~SpriteNode() = default;

    void SpriteNode::Init() {
        Node::Init();
    }

    void SpriteNode::SystemProcess() {
        Node::SystemProcess();
        if (m_pSprite != nullptr) {
            switch (m_spriteDisplayMode) {
                case Original:
                    break;
                case Fit:
                    m_pSprite->SetWidth(m_globalTransform.GetWidth());
                    m_pSprite->SetHeight(m_globalTransform.GetHeight());
                    break;
                case Scale:
                    // only the hight can be configured to scale the sprite
                    m_aspectRatio.x = std::round(m_pSprite->GetWidth() / m_pSprite->GetHeight() * 100) / 100;
                    m_aspectRatio.y = 1;
                    m_globalTransform.SetWidth(m_globalTransform.GetHeight() * m_aspectRatio.x);

                    m_pSprite->SetWidth(m_globalTransform.GetWidth());
                    m_pSprite->SetHeight(m_globalTransform.GetHeight());
                    break;
            }
            m_pSprite->SetX(m_globalTransform.position.x + m_pSprite->GetWidth() / 2);
            m_pSprite->SetY(m_globalTransform.position.y + m_pSprite->GetHeight() / 2);
            m_pSprite->SetAngle(m_globalTransform.GetRotation());
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

        if (m_pSprite == nullptr && m_pRenderer != nullptr) {
            m_pSprite = m_pRenderer->CreateSprite(m_pSpritePath);
            m_pSprite->m_iLayer = 0;
        }
    }


    /**
     * All parameters take values between 0 and 1. Highter/lower values will be set to max 1 and min 0.
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

    void SpriteNode::SetSpritePath(const char* path) {
        m_pSpritePath = path;
    }
}
