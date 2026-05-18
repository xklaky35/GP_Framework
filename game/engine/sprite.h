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
        [[nodiscard]] virtual float GetWidth() const;
        [[nodiscard]] virtual float GetHeight() const;

        [[nodiscard]] float GetOriginalWidth() const;
        [[nodiscard]] float GetOriginalHeight() const;
        [[nodiscard]] float GetX() const;
        [[nodiscard]] float GetY() const;
        [[nodiscard]] float GetAngle() const;
        [[nodiscard]] float GetGreenTint() const;
        [[nodiscard]] float GetAlpha() const;
        [[nodiscard]] float GetRedTint() const;
        [[nodiscard]] float GetBlueTint() const;
        [[nodiscard]] float GetScale() const;

        void SetHeight(float x);
        void SetX(float x);
        void SetY(float y);
        void SetAngle(float angle);
        void SetWidth(float x);
        void SetScale(float scale);
        void SetAlpha(float alpha);
        void SetRedTint(float value);
        void SetGreenTint(float value);
        void SetBlueTint(float value);
        void SetActive() const;

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
