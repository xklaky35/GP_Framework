#include "animatedspritenode.h"

#include <cassert>

#include "imgui.h"
#include "../animatedsprite.h"

namespace Engine {
    AnimatedSpriteNode::AnimatedSpriteNode() : m_bIsLooping(false), m_bIsAnimating(false), m_fFrameDuration(0),
                                               m_frameHeight(0),
                                               m_frameWidth(0) {
        m_name = "AnimatedSpriteNode";
        m_nodeInfo.push_back({
            "Frame Duration", [](Node &n) {
                auto *animNode = dynamic_cast<AnimatedSpriteNode *>(&n);
                if (animNode) {
                    auto *animSprite = dynamic_cast<AnimatedSprite *>(animNode->m_pSprite);
                    int v_min = 0, v_max = animSprite->m_iTotalFrames;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &animSprite->m_frameDuration, 1, 0.5f,
                                           &v_min, &v_max)) {
                        animNode->m_iniParser->SetValue("AnimatedSpriteNode", "frameDuration",
                                                        animSprite->m_frameDuration);
                    }
                }
            }
        });
        m_nodeInfo.push_back({
            "Frame Width", [](Node &n) {
                auto *animNode = dynamic_cast<AnimatedSpriteNode *>(&n);
                if (animNode) {
                    auto *animSprite = dynamic_cast<AnimatedSprite *>(animNode->m_pSprite);
                    int v_min = 0, v_max = 10000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &animNode->m_frameWidth, 1, 0.5f,
                                           &v_min, &v_max)) {
                        animNode->m_iniParser->SetValue("AnimatedSpriteNode", "frameWidth",
                                                        animNode->m_frameWidth);
                        animNode->SetupFrames();
                    }
                }
            }
        });
        m_nodeInfo.push_back({
            "Frame Height", [](Node &n) {
                auto *animNode = dynamic_cast<AnimatedSpriteNode *>(&n);
                if (animNode) {
                    auto *animSprite = dynamic_cast<AnimatedSprite *>(animNode->m_pSprite);
                    int v_min = 0, v_max = 10000;
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &animNode->m_frameHeight, 1, 0.5f,
                                           &v_min, &v_max)) {
                        animNode->m_iniParser->SetValue("AnimatedSpriteNode", "frameHeight",
                                                        animNode->m_frameHeight);
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
                    if (ImGui::Checkbox("##Editor", &animSprite->m_bLooping)) {
                        animNode->m_iniParser->SetValue("AnimatedSpriteNode", "isLooping", animSprite->m_bLooping);
                    }
                }
            }
        });
        m_nodeInfo.push_back({
            "Is animating", [](Node &n) {
                auto *animNode = dynamic_cast<AnimatedSpriteNode *>(&n);
                if (animNode) {
                    auto *animSprite = dynamic_cast<AnimatedSprite *>(animNode->m_pSprite);
                    if (ImGui::Checkbox("##Editor", &animSprite->m_bAnimating)) {
                        animNode->m_iniParser->SetValue("AnimatedSpriteNode", "isAnimating", animSprite->m_bAnimating);
                    }
                }
            }
        });
        m_nodeInfo.push_back({
            "Is flipped", [](Node &n) {
                auto *animNode = dynamic_cast<AnimatedSpriteNode *>(&n);
                if (animNode) {
                    auto *animSprite = dynamic_cast<AnimatedSprite *>(animNode->m_pSprite);
                    if (ImGui::Checkbox("##Editor", &animSprite->m_bIsFlipped)) {
                        animNode->m_iniParser->SetValue("AnimatedSpriteNode", "isFlipped", animSprite->m_bIsFlipped);
                        animNode->SetupFrames();
                    }
                }
            }
        });
    }

    /**
     * This node uses the sprites dimensions by default. If you specify a custom height and width, please disable m_bUseSpriteSize.
     * @param iniParser path to the data file the data should be written to
     */
    AnimatedSpriteNode::AnimatedSpriteNode(IniParser* iniParser) : AnimatedSpriteNode() {
        m_iniParser = iniParser;
    }

    void AnimatedSpriteNode::Init() {
        SpriteNode::Init();

    }

    void AnimatedSpriteNode::Process(float deltaTime) {
        SpriteNode::Process(deltaTime);

        if (auto* animSprite = dynamic_cast<AnimatedSprite *>(m_pSprite)) {
            animSprite->Process(deltaTime);
        }
    }

    void AnimatedSpriteNode::Draw(Renderer &renderer) {
        SpriteNode::Draw(renderer);

        if (auto* animSprite = dynamic_cast<AnimatedSprite *>(m_pSprite)) {
            animSprite->Draw(renderer);
        }
    }

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

    void AnimatedSpriteNode::SetupFrames() {
        if (auto* animSprite = dynamic_cast<AnimatedSprite*>(m_pSprite)) {
            animSprite->SetupFrames(m_frameWidth,m_frameHeight);
        }
    }
    void AnimatedSpriteNode::SetLooping(bool isLooping) {
        if (auto* animSprite = dynamic_cast<AnimatedSprite *>(m_pSprite)) {
            animSprite->SetLooping(isLooping);
        }
    }

    void AnimatedSpriteNode::SetAnimating(bool isAnimating) {
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

    void AnimatedSpriteNode::Restart() {
        if (auto* animSprite = dynamic_cast<AnimatedSprite *>(m_pSprite)) {
            return animSprite->Restart();
        }
    }

    void AnimatedSpriteNode::SetFrameDuration(float seconds) {
        if (auto* animSprite = dynamic_cast<AnimatedSprite *>(m_pSprite)) {
            return animSprite->SetFrameDuration(seconds);
        }
    }

    void AnimatedSpriteNode::Flip() {
        if (auto* animSprite = dynamic_cast<AnimatedSprite *>(m_pSprite)) {
            animSprite->m_bIsFlipped = !animSprite->m_bIsFlipped;
            SetupFrames();
        }
    }
}
