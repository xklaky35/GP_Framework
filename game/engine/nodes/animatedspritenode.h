#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

// Local includes:
#include "spritenode.h"


namespace Engine {
    // Forward declarations:
    class Renderer;
    class VertexArray;

    // Class declaration:
    class AnimatedSprite : public SpriteNode {
        // Member methods:
    public:
        AnimatedSprite(const char* path, int,int);

        ~AnimatedSprite() override;

        void Init() override;
        void Process(float deltaTime) override;
        void Draw(Renderer &renderer) override;
        void SetLooping(bool loop);
        void StopAnimate();
        void Animate();
        bool IsAnimating() const;
        void Restart();
        void SetFrameDuration(float seconds);
        void DebugDraw();

    private:
        void SetupFrames();
    private:
        AnimatedSprite(const AnimatedSprite &animatedsprite);
        AnimatedSprite &operator=(const AnimatedSprite &animatedsprite);

        // Member data:
    public:
    protected:
        VertexArray *m_pVertexData;
        int m_iFrameWidth;
        int m_iFrameHeight;
        int m_iCurrentFrame;
        int m_iTotalFrames;
        float m_fTimeElapsed;
        float m_frameDuration;
        float totalTime;
        bool m_bAnimating;
        bool m_bLooping;
    };
}
#endif
