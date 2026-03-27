// COMP710 GP Framework 2025
#ifndef SPRITE_H
#define SPRITE_H

#include "texture.h"

namespace Engine {
    class Renderer;

    class Sprite {
        // Member methods:
    public:
        Sprite();
        virtual ~Sprite();
        virtual bool Initialise(Texture &texture);
        virtual void Process(float deltaTime);
        virtual void Draw(Renderer &renderer);
        virtual void DrawDebug();
        virtual float GetWidth() const;

        float GetOriginalWidth() const;

        void SetWidth(float x);
        virtual float GetHeight() const;

        float GetOriginalHeight() const;

        void SetHeight(float x);
        void SetX(float x);
        float GetX() const;
        void SetY(float y);
        float GetY() const;
        void SetAngle(float angle);
        float GetAngle() const;
        void SetScale(float scale);
        float GetScale() const;
        void SetAlpha(float alpha);
        float GetAlpha() const;
        void SetRedTint(float value);
        float GetRedTint() const;
        void SetGreenTint(float value);
        float GetGreenTint() const;
        void SetBlueTint(float value);
        float GetBlueTint() const;
        void SetActive();

    protected:
        float Clamp(float minimum, float value, float maximum);


    public:
        int m_iLayer;

    protected:
        Texture *m_pTexture;
        float m_x;
        float m_y;
        float m_angle;
        float m_centerX;
        float m_centerY;
        float m_width;
        float m_height;
        float m_originalWidth;
        float m_originalHeight;
        float m_scale;
        float m_alpha;
        float m_tintRed;
        float m_tintGreen;
        float m_tintBlue;

    };
}
#endif // SPRITE_H
