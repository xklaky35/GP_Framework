#include "spritenode.h"

namespace Engine {
    /**
     * This node uses the sprites dimensions by default. If you specify a custom height and width, please disable m_bUseSpriteSize.
     * @param nodeName name of the node (this is set by default)
     */
    SpriteNode::SpriteNode(const char *nodeName) : Node(nodeName), m_bUseSpriteSize(true), m_bCanDeform(true),
                                                   m_pRenderer(nullptr),
                                                   m_pSprite(nullptr), m_redTint(1), m_greenTint(1), m_blueTint(1),
                                                   m_alpha(1) {
    }

    /**
     * This node uses the sprites dimensions by default. If you specify a custom height and width, please disable m_bUseSpriteSize.
     * @param spritePath path to the sprite image
     * @param nodeName name of the node (this is set by default)
     */
    SpriteNode::SpriteNode(const char *spritePath, const char *nodeName) : Node(nodeName), m_bUseSpriteSize(true),
                                                                           m_bCanDeform(true),
                                                                           m_pRenderer(nullptr),
                                                                           m_pSprite(nullptr),
                                                                           m_pSpritePath(spritePath), m_redTint(1),
                                                                           m_greenTint(1),
                                                                           m_blueTint(1), m_alpha(1) {
    }

    /**
     * This node uses the sprites dimensions by default. If you specify a custom height and width, please disable m_bUseSpriteSize.
     * @param height hight the sprite should be rendered with
     * @param width width the sprite should be rendered with
     * @param spritePath path to the sprite image
     * @param nodeName name of the node (this is set by default)
     */
    SpriteNode::SpriteNode(float height, float width, const char *spritePath, const char *nodeName) : Node(nodeName),
        m_bUseSpriteSize(true), m_bCanDeform(true),
        m_pRenderer(nullptr),
        m_pSprite(nullptr),
        m_pSpritePath(spritePath),
        m_redTint(1),
        m_greenTint(1),
        m_blueTint(1),
        m_alpha(1) {
        m_transform->height = height;
        m_transform->width = width;
    }

    SpriteNode::~SpriteNode() = default;

    void SpriteNode::Init() {
        Node::Init();
    }

    void SpriteNode::Process(float deltaTime) {
        Node::Process(deltaTime);
        if (m_pSprite != nullptr) {
            m_pSprite->SetX(m_position->x);
            m_pSprite->SetY(m_position->y);

            if (m_bUseSpriteSize == false) {
                if (m_bCanDeform == true) {
                    m_pSprite->SetWidth(m_transform->width);
                    m_pSprite->SetHeight(m_transform->height);
                } else {
                    float scaleFactor = 1 / (m_pSprite->GetWidth() / m_transform->width);
                    m_pSprite->SetWidth(m_pSprite->GetWidth() * scaleFactor);
                    m_pSprite->SetHeight(m_pSprite->GetHeight() * scaleFactor);
                }
            }
            m_pSprite->SetScale(m_transform->scale);
            m_pSprite->SetAngle(m_transform->rotation);
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
