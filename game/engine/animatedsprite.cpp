
#include "animatedsprite.h"
// Local includes:
#include "renderer.h"
#include "texture.h"
#include "imgui.h"
// Library includes:
#include <cassert>

namespace Engine {

    AnimatedSprite::AnimatedSprite()
        : m_iCurrentFrame(0)
          , m_frameDuration(1.0f)
          , m_bAnimating(true)
          , m_bLooping(true)
          , m_iTotalFrames(0)
          , m_bIsFlipped(false), m_iFrameWidth(0)
          , m_iFrameHeight(0)
          , m_pVertexData(nullptr)
          , m_fTimeElapsed(0.0f)
          , totalTime(0.0f) {
    }

    AnimatedSprite::~AnimatedSprite()
    {
        delete m_pVertexData;
        m_pVertexData = nullptr;
    }

    bool AnimatedSprite::Initialise(Texture& texture)
    {
        if (!Sprite::Initialise(texture)) {
            return false;
        }
        return true;
    }
    float AnimatedSprite::GetWidth() const
    {
        return ceilf(static_cast<float>(m_iFrameWidth) * m_scale);
    }
    float AnimatedSprite::GetHeight() const
    {
        return ceilf(static_cast<float>(m_iFrameHeight) * m_scale);
    }
    bool AnimatedSprite::IsAnimating() const
    {
        return m_bAnimating;
    }
    void AnimatedSprite::SetupFrames(int fixedFrameWidth, int fixedFrameHeight)
    {
        if (fixedFrameWidth <= 0 || fixedFrameHeight <= 0) return;

        m_iFrameWidth = fixedFrameWidth;
        m_iFrameHeight = fixedFrameHeight;
        const int textureWidth = m_pTexture->GetWidth();
        const int textureHeight = m_pTexture->GetHeight();
        const int totalFramesWide = textureWidth / fixedFrameWidth;
        const int totalFramesHigh = textureHeight / fixedFrameHeight;
        constexpr int stride = 5;
        const float uFrameWidth = 1.0f / static_cast<float>(totalFramesWide);
        const float vFrameHeight = 1.0f / static_cast<float>(totalFramesHigh);
        m_iTotalFrames = totalFramesWide * totalFramesHigh;
        constexpr int vertsPerSprite = 4;
        const int numVertices = vertsPerSprite * (m_iTotalFrames);
        auto vertices = new float[numVertices * stride];

        auto isFlipped = static_cast<float>(m_bIsFlipped);
        auto isNotFlipped = static_cast<float>(!m_bIsFlipped);

        for (int h = 0; h < totalFramesHigh; ++h)
        {
            for (int w = 0; w < totalFramesWide; ++w)
            {
                float uOffset = static_cast<float>(w) * uFrameWidth;
                float vOffset = static_cast<float>(h) * vFrameHeight;
                float quad[] =
                {
                    // x      y      z      u                      v
                    -0.5f,  0.5f,  0.0f,  uOffset + (uFrameWidth *  isFlipped),   vOffset + vFrameHeight,  // top-left
                     0.5f,  0.5f,  0.0f,  uOffset + (uFrameWidth * isNotFlipped), vOffset + vFrameHeight,  // top-right
                     0.5f, -0.5f,  0.0f,  uOffset + (uFrameWidth * isNotFlipped), vOffset ,                // bottom-right
                    -0.5f, -0.5f,  0.0f,  uOffset + (uFrameWidth *  isFlipped),   vOffset ,                // bottom-left
                };

                constexpr int floatsPerSprite = stride * vertsPerSprite;
                for (int j = 0; j < floatsPerSprite; ++j)
                {
                    int index = ((w * floatsPerSprite) + j) + (h * (floatsPerSprite * totalFramesWide));
                    vertices[index] = quad[j];
                }
            }
        }
        const int totalIndices = 6 * m_iTotalFrames;
        auto* allIndices = new unsigned int[totalIndices];
        unsigned int i = 0;
        for (int k = 0; k < m_iTotalFrames; ++k)
        {
            const unsigned int indices[] = { 0 + i, 1 + i, 2 + i, 2 + i, 3 + i, 0 + i };
            for (int j = 0; j < 6; ++j)
            {
                const int index = (k * 6) + j;
                allIndices[index] = indices[j];
            }
            i += 4;
        }
        m_pVertexData = new VertexArray(vertices, numVertices, allIndices, totalIndices);
        delete[] vertices;
        vertices = nullptr;
        delete[] allIndices;
        allIndices = nullptr;
    }

    void AnimatedSprite::Process(float deltaTime)
    {
        totalTime += deltaTime;
        if (m_bAnimating)
        {
            m_fTimeElapsed += deltaTime;
            if (m_fTimeElapsed > m_frameDuration)
            {
                ++m_iCurrentFrame;
                if (m_iCurrentFrame >= m_iTotalFrames)
                {
                    if (m_bLooping)
                    {
                        Restart();
                    }
                    else
                    {
                        m_iCurrentFrame = m_iTotalFrames - 1;
                        m_bAnimating = false;
                    }
                }
                m_fTimeElapsed = 0.0f;
            }
        }
    }

    void AnimatedSprite::Draw(Renderer& renderer)
    {
        if (m_pVertexData != nullptr) {
            renderer.DrawSprite(*this);
        }
    }

    void AnimatedSprite::DrawDebug() {
        Sprite::DrawDebug();
        ImGui::SliderInt("Frame ", &m_iCurrentFrame, 0, m_iTotalFrames - 1);
    }

    void AnimatedSprite::Animate()
    {
        m_bAnimating = true;
    }

    void AnimatedSprite::SetFrameDuration(float seconds)
    {
        m_frameDuration = seconds;
    }

    void AnimatedSprite::SetLooping(bool loop)
    {
        m_bLooping = loop;
    }
    void AnimatedSprite::Restart()
    {
        m_iCurrentFrame = 0;
        m_fTimeElapsed = 0.0f;
    }

}