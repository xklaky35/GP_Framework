#ifndef GP_FRAMEWORK_SPRITENODE_H
#define GP_FRAMEWORK_SPRITENODE_H

#include "node.h"
#include "../renderer.h"

namespace Engine {
    class SpriteNode : public Node {
    public:
        SpriteNode(const char* = "Sprite");
        SpriteNode(const char* spritePath, const char* = "Sprite");
        SpriteNode(float height, float weight, const char* spritePath, const char* = "Sprite");
        ~SpriteNode() override;

        void Init() override;
        void Process(float deltaTime) override;
        void Draw(Renderer &) override;
        void DrawDebug() override;
        void SetRGBA(float,float,float,float);
        void SetSpritePath(const std::string &);

    private:
        void SetupSpriteRendering(Renderer&);

    public:
        bool m_bUseSpriteSize;

    private:
        Renderer *m_pRenderer;
        Sprite *m_pSprite;
        std::string m_pSpritePath;
        float m_redTint;
        float m_greenTint;
        float m_blueTint;
        float m_alpha;
    };
}

#endif //GP_FRAMEWORK_SPRITENODE_H