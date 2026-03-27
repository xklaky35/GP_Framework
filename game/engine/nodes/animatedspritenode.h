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
        AnimatedSpriteNode(IniParser* parser);

        void Init() override;
        void Process(float deltaTime) override;
        void Draw(Renderer &renderer) override;
        void SetupSpriteRendering(Renderer& renderer) override;
        void SetLooping(bool);
        void SetAnimating(bool);
        bool IsAnimating() const;
        void Restart();
        void SetFrameDuration(float seconds);

    private:
        AnimatedSpriteNode(const AnimatedSpriteNode &animatedsprite);
        AnimatedSpriteNode &operator=(const AnimatedSpriteNode &animatedsprite);

        // Member data:
    public:
        bool m_bIsLooping;
        bool m_bIsAnimating;
        float m_fFrameDuration;
    };
}
#endif
