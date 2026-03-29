//
// Created by leon on 27.03.26.
//

#ifndef GP_FRAMEWORK_ANIMATEDSPRITE_H
#define GP_FRAMEWORK_ANIMATEDSPRITE_H

// Local includes:
#include "sprite.h"
#include "structs/vertexarray.h"
// Forward declarations:

namespace Engine {
    class AnimatedSprite : public Sprite {
    public:
        AnimatedSprite();
        ~AnimatedSprite() override;

        bool Initialise(Texture &texture) override;
        void Process(float deltaTime) override;
        void Draw(Renderer &renderer) override;
        void DrawDebug() override;
        float GetWidth() const override;
        float GetHeight() const override;

        void SetupFrames(int fixedFrameWidth, int fixedFrameHeight);
        void SetLooping(bool loop);
        void Animate();
        bool IsAnimating() const;
        void Restart();
        void SetFrameDuration(float seconds);

    protected:
    private:
        AnimatedSprite(const AnimatedSprite &animatedsprite);
        AnimatedSprite &operator=(const AnimatedSprite &animatedsprite);

        // Member data:
    public:
        int m_iCurrentFrame;
        float m_frameDuration;
        bool m_bAnimating;
        bool m_bLooping;
        int m_iTotalFrames;
        bool m_bIsFlipped;
        int m_iFrameWidth;
        int m_iFrameHeight;
    protected:
        VertexArray *m_pVertexData;
        float m_fTimeElapsed;
        float totalTime;

    private:
    };
}

#endif //GP_FRAMEWORK_ANIMATEDSPRITE_H