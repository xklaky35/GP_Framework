#include "animatedspritenode.h"

#include "imgui.h"
#include "nodefactory.h"
#include "../animatedsprite.h"

namespace Engine {
    AnimatedSpriteNode::AnimatedSpriteNode() : m_bIsLooping(false), m_bIsAnimating(false), m_fFrameDuration(0),
                                               m_iFrameHeight(0),
                                               m_iFrameWidth(0) {
        SetupNode("AnimatedSpriteNode", NT_AnimatedSpriteNode);
        m_nodeInfo.push_back({
            "Frame Duration", [](Node &n) {
                auto *animNode = dynamic_cast<AnimatedSpriteNode *>(&n);
                if (animNode) {
                    auto *animSprite = dynamic_cast<AnimatedSprite *>(animNode->m_pSprite);
                    if (animSprite == nullptr) return;
                    int v_min = 0, v_max = animSprite->m_iTotalFrames;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &animSprite->m_frameDuration, 1, 0.5f,
                                           &v_min, &v_max)) {
                        animNode->SetValue("frameDuration",animSprite->m_frameDuration);
                    }
                }
            }
        });
        m_nodeInfo.push_back({
            "Frame Width", [](Node &n) {
                auto *animNode = dynamic_cast<AnimatedSpriteNode *>(&n);
                if (animNode) {
                    int v_min = 0, v_max = 10000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_S32, &animNode->m_iFrameWidth, 1, 0.5f,
                                           &v_min, &v_max)) {
                        animNode->SetValue("frameWidth",animNode->m_iFrameWidth);
                        animNode->SetupFrames();
                    }
                }
            }
        });
        m_nodeInfo.push_back({
            "Frame Height", [](Node &n) {
                auto *animNode = dynamic_cast<AnimatedSpriteNode *>(&n);
                if (animNode) {
                    int v_min = 0, v_max = 10000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_S32, &animNode->m_iFrameHeight, 1, 0.5f,
                                           &v_min, &v_max)) {
                        animNode->SetValue("frameHeight",animNode->m_iFrameHeight);
                        animNode->SetupFrames();
                    }
                }
            }
        });
        m_nodeInfo.push_back({
            "Is looping", [](Node &n) {
                auto *animNode = dynamic_cast<AnimatedSpriteNode *>(&n);
                if (animNode) {
                    auto *animSprite = dynamic_cast<AnimatedSprite *>(animNode->m_pSprite);
                    if (animSprite == nullptr) return;
                    if (ImGui::Checkbox("##Editor", &animSprite->m_bLooping)) {
                        animNode->SetValue("isLooping", animSprite->m_bLooping);
                    }
                }
            }
        });
        m_nodeInfo.push_back({
            "Is animating", [](Node &n) {
                auto *animNode = dynamic_cast<AnimatedSpriteNode *>(&n);
                if (animNode) {
                    auto *animSprite = dynamic_cast<AnimatedSprite *>(animNode->m_pSprite);
                    if (animSprite == nullptr) return;
                    if (ImGui::Checkbox("##Editor", &animSprite->m_bAnimating)) {
                        animNode->SetValue("isAnimating", animSprite->m_bAnimating);
                    }
                }
            }
        });
        m_nodeInfo.push_back({
            "Is flipped", [](Node &n) {
                auto *animNode = dynamic_cast<AnimatedSpriteNode *>(&n);
                if (animNode) {
                    auto *animSprite = dynamic_cast<AnimatedSprite *>(animNode->m_pSprite);
                    if (animSprite == nullptr) return;
                    if (ImGui::Checkbox("##Editor", &animSprite->m_bIsFlipped)) {
                        animNode->SetValue("isFlipped", animSprite->m_bIsFlipped);
                        animNode->SetupFrames();
                    }
                }
            }
        });
    }

    void AnimatedSpriteNode::Init() {
        SpriteNode::Init();

    }

    void AnimatedSpriteNode::SetupParameter(IniParser *parser, const std::string &section) {
        SpriteNode::SetupParameter(parser, section);
        m_bIsLooping = parser->GetValueAsBoolean(section, "isLooping");
        m_bIsAnimating = parser->GetValueAsBoolean(section, "isAnimating");
        m_fFrameDuration = parser->GetValueAsFloat(section, "frameDuration");
        m_iFrameWidth = parser->GetValueAsInt(section, "frameWidth");
        m_iFrameHeight = parser->GetValueAsInt(section, "frameHeight");
    }

    void AnimatedSpriteNode::Process(float deltaTime) {
        SpriteNode::Process(deltaTime);

        if (auto* animSprite = dynamic_cast<AnimatedSprite *>(m_pSprite)) {
            animSprite->Process(deltaTime);
            m_bIsAnimating = animSprite->m_bAnimating;
        }
    }

    void AnimatedSpriteNode::Draw(Renderer &renderer) {
        SpriteNode::Draw(renderer);

        if (auto* animSprite = dynamic_cast<AnimatedSprite *>(m_pSprite)) {
            animSprite->Draw(renderer);
        }
    }
//../assets/Sprites/BlueWizardSprites/MossySpritesSingle/Wall.png
    void AnimatedSpriteNode::SetupSpriteRendering(Renderer &renderer) {
        if (m_pRenderer == nullptr) {
            m_pRenderer = &renderer;
        }

        if (!m_spritePath.empty() && m_pSprite == nullptr && m_pRenderer != nullptr) {
            m_pSprite = m_pRenderer->CreateAnimatedSprite(m_spritePath.c_str());
            m_pSprite->m_iLayer = m_iLayer;
            SetLooping(m_bIsLooping);
            SetAnimating(m_bIsAnimating);
            SetFrameDuration(m_fFrameDuration);
            SetupFrames();
        }
    }

    void AnimatedSpriteNode::SetupFrames() const {
        if (auto* animSprite = dynamic_cast<AnimatedSprite*>(m_pSprite)) {
            animSprite->SetupFrames(m_iFrameWidth,m_iFrameHeight);
        }
    }
    void AnimatedSpriteNode::SetLooping(bool isLooping) const {
        if (auto* animSprite = dynamic_cast<AnimatedSprite *>(m_pSprite)) {
            animSprite->SetLooping(isLooping);
        }
    }

    void AnimatedSpriteNode::SetAnimating(bool isAnimating) const {
        if (auto* animSprite = dynamic_cast<AnimatedSprite *>(m_pSprite)) {
            animSprite->m_bAnimating = isAnimating;
        }
    }
    bool AnimatedSpriteNode::IsAnimating() const {
        if (auto* animSprite = dynamic_cast<AnimatedSprite *>(m_pSprite)) {
            return animSprite->IsAnimating();
        }
        return false;
    }

    void AnimatedSpriteNode::Restart() const {
        if (auto* animSprite = dynamic_cast<AnimatedSprite *>(m_pSprite)) {
            return animSprite->Restart();
        }
    }

    void AnimatedSpriteNode::SetFrameDuration(float seconds) const {
        if (auto* animSprite = dynamic_cast<AnimatedSprite *>(m_pSprite)) {
            return animSprite->SetFrameDuration(seconds);
        }
    }

    void AnimatedSpriteNode::Flip() const {
        if (auto* animSprite = dynamic_cast<AnimatedSprite *>(m_pSprite)) {
            animSprite->m_bIsFlipped = !animSprite->m_bIsFlipped;
            SetupFrames();
        }
    }

    bool AnimatedSpriteNode::IsFlipped() const {
        if (auto* animSprite = dynamic_cast<AnimatedSprite *>(m_pSprite)) {
            return animSprite->m_bIsFlipped;
        }
        return false;
    }
}
