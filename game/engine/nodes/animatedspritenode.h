#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

// Local includes:
#include "spritenode.h"


namespace Engine {
    class AnimatedSprite;
    // Forward declarations:
    class Renderer;
    class VertexArray;

    // Class declaration:
    class AnimatedSpriteNode : public SpriteNode {
        // Member methods:
    public:
        AnimatedSpriteNode();

        void Init() override;
        void SetupParameter(IniParser *, const std::string &) override;
        void Process(float deltaTime) override;
        void Draw(Renderer &renderer) override;
        void SetupSpriteRendering(Renderer& renderer) override;

        void SetupFrames();

        void SetLooping(bool);
        void SetAnimating(bool);
        bool IsAnimating() const;
        void Restart();
        void SetFrameDuration(float seconds);
        void Flip();

        bool IsFlipped();

    private:
        AnimatedSpriteNode(const AnimatedSpriteNode &animatedsprite);
        AnimatedSpriteNode &operator=(const AnimatedSpriteNode &animatedsprite);

        // Member data:
    public:
        bool m_bIsLooping;
        bool m_bIsAnimating;
        float m_fFrameDuration;
        int m_frameHeight;
        int m_frameWidth;
    };
}
#endif
