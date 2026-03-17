#include "spritenode.h"

#include <numeric>

namespace Engine {
    /**
     * This node uses the sprites dimensions by default.
     */
    SpriteNode::SpriteNode() : m_spriteDisplayMode(Original),
                               m_pSprite(nullptr),
                               m_pRenderer(nullptr), m_redTint(1), m_greenTint(1), m_blueTint(1),
                               m_alpha(1) {
    }

    /**
     * This node uses the sprites dimensions by default. If you specify a custom height and width, please disable m_bUseSpriteSize.
     * @param spritePath path to the sprite image
     */
    SpriteNode::SpriteNode(const char *spritePath) : m_spriteDisplayMode(Original), m_pSprite(nullptr),
                                                     m_pRenderer(nullptr),
                                                     m_pSpritePath(spritePath), m_redTint(1),
                                                     m_greenTint(1),
                                                     m_blueTint(1), m_alpha(1) {
    }

    /**
     * This node uses the sprites dimensions by default. If you specify a custom height and width, please disable m_bUseSpriteSize.
     * @param height hight the sprite should be rendered with
     * @param width width the sprite should be rendered with
     * @param spritePath path to the sprite image
     */
    SpriteNode::SpriteNode(float height, float width, const char *spritePath) : m_spriteDisplayMode(Fit),
        m_pRenderer(nullptr),
        m_pSprite(nullptr),
        m_pSpritePath(spritePath),
        m_redTint(1),
        m_greenTint(1),
        m_blueTint(1),
        m_alpha(1),
        m_scaleFactor(0) {
        m_globalTransform.SetHeight(height);
        m_globalTransform.SetWidth(width);
    }

    SpriteNode::~SpriteNode() = default;

    void SpriteNode::Init() {
        Node::Init();
    }

    void SpriteNode::Process(float deltaTime) {
        Node::Process(deltaTime);
        if (m_pSprite != nullptr) {
            m_pSprite->SetX(m_globalTransform.position.x);
            m_pSprite->SetY(m_globalTransform.position.y);

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
            //m_pSprite->SetScale(m_globalTransform.GetScale());
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

    void SpriteNode::DrawDebug() {}

    void SpriteNode::SetupSpriteRendering(Renderer &renderer) {
        if (m_pRenderer == nullptr) {
            m_pRenderer = &renderer;
        }

        if (m_pSprite == nullptr && m_pRenderer != nullptr) {
            m_pSprite = m_pRenderer->CreateSprite(m_pSpritePath.c_str());
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

    void SpriteNode::SetSpritePath(const std::string &path) {
        m_pSpritePath = path;
    }
}
