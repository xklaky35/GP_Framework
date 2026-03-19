#include "sprite.h"
#include "renderer.h"

namespace Engine {
    Sprite::Sprite()
        : m_iLayer(0), m_pTexture(nullptr)
          , m_x(0)
          , m_y(0)
          , m_width(0)
          , m_height(0)
          , m_angle(0.0f)
          , m_centerX(0)
          , m_centerY(0)
          , m_scale(1.0f)
          , m_alpha(1.0f)
          , m_tintRed(1.0f)
          , m_tintGreen(1.0f)
          , m_tintBlue(1.0f) {
    }

    Sprite::~Sprite() = default;

    bool
    Sprite::Initialise(Texture &texture) {
        m_pTexture = &texture;
        m_width = m_pTexture->GetWidth();
        m_height = m_pTexture->GetHeight();
        return true;
    }

    void Sprite::Process(float deltaTime) {}

    void Sprite::Draw(Renderer &renderer) {
        renderer.DrawSprite(*this);
    }

    void Sprite::SetX(float x) {
        m_x = x;
    }

    void Sprite::SetY(float y) {
        m_y = y;
    }

    float Sprite::GetX() const {
        return m_x;
    }

    float Sprite::GetY() const {
        return m_y;
    }

    void Sprite::SetAngle(float angle) {
        while (angle > 360) {
            angle -= 360;
        }
        m_angle = angle;
    }

    float Sprite::GetAngle() const {
        return m_angle;
    }

    //
    void Sprite::SetScale(float scale) {
        m_scale = scale;
    }

    float Sprite::GetScale() const {
        return m_scale;
    }

    void Sprite::SetAlpha(float alpha) {
        m_alpha = Clamp(0.0f, alpha, 1.0f);
    }

    float Sprite::GetAlpha() const {
        return m_alpha;
    }

    float Sprite::GetWidth() const {
        return static_cast<int>(ceilf(m_width * m_scale));
    }

    void Sprite::SetWidth(const float w) {
        m_width = w;
    }

    void Sprite::SetHeight(const float h) {
        m_height = h;
    }

    float Sprite::GetHeight() const {
        return static_cast<int>(ceilf(m_height * m_scale));
    }

    float Sprite::Clamp(float minimum, float value, float maximum) {
        if (value > maximum) {
            value = maximum;
        } else if (value < minimum) {
            value = minimum;
        }
        return value;
    }

    void Sprite::SetRedTint(float value) {
        m_tintRed = Clamp(0.0f, value, 1.0f);
    }

    float Sprite::GetRedTint() const {
        return m_tintRed;
    }

    void Sprite::SetGreenTint(float value) {
        m_tintGreen = Clamp(0.0f, value, 1.0f);
    }

    float Sprite::GetGreenTint() const {
        return m_tintGreen;
    }

    void Sprite::SetBlueTint(float value) {
        m_tintBlue = Clamp(0.0f, value, 1.0f);
    }

    float Sprite::GetBlueTint() const {
        return m_tintBlue;
    }

    void Sprite::SetActive() {
        m_pTexture->SetActive();
    }
}
