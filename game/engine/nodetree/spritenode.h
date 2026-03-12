#ifndef GP_FRAMEWORK_SPRITENODE_H
#define GP_FRAMEWORK_SPRITENODE_H

#include "node.h"
#include "../renderer.h"

namespace Engine {
    class Spritenode : public Node {
    public:
        Spritenode(const char* = "Sprite");
        Spritenode(const char* spritePath, const char* = "Sprite");
        Spritenode(float height, float weight, const char* spritePath, const char* = "Sprite");
        ~Spritenode() override;

        void Init() override;
        void Process(float deltaTime) override;
        void Draw(Renderer &) override;
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