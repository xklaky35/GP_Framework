#include "animatedspritenode.h"

#include "imgui.h"
namespace Engine {

    AnimatedSprite::AnimatedSprite(const char *path, int frameHight, int frameWidth) : m_pVertexData(nullptr),
                                                 m_iFrameWidth(frameWidth),
                                                 m_iFrameHeight(frameHight),
                                                 m_iCurrentFrame(0),
                                                 m_iTotalFrames(0),
                                                 m_fTimeElapsed(0),
                                                 m_frameDuration(0), totalTime(0),
                                                 m_bAnimating(false),
                                                 m_bLooping(false) {
    }

    AnimatedSprite::~AnimatedSprite() {
        delete m_pVertexData;
        m_pVertexData = nullptr;
    }

    bool AnimatedSprite::IsAnimating() const {
        return m_bAnimating;
    }

    void AnimatedSprite::SetupFrames() {
        const int textureWidth = m_pSprite->GetWidth();
        const int textureHeight = m_pSprite->GetHeight();
        const int totalFramesWide = textureWidth / m_iFrameWidth;
        const int totalFramesHigh = textureHeight / m_iFrameHeight;

        const int stride = 5;
        const float uFrameWidth = 1.0f / totalFramesWide;
        const float vFrameHeight = 1.0f / totalFramesHigh;
        m_iTotalFrames = totalFramesWide * totalFramesHigh;

        const int vertsPerSprite = 4;
        const int numVertices = vertsPerSprite * (m_iTotalFrames);

        float *vertices = new float[numVertices * stride];
        for (int h = 0; h < totalFramesHigh; ++h) {
            for (int w = 0; w < totalFramesWide; ++w) {
                float uOffset = (w * uFrameWidth);
                float vOffset = (h * vFrameHeight);
                float quad[] =
                {
                    -0.5f,  0.5f, 0.0f,        0.0f + uOffset, vOffset + vFrameHeight,
                     0.5f,  0.5f, 0.0f, uFrameWidth + uOffset, vOffset + vFrameHeight,
                     0.5f, -0.5f, 0.0f, uFrameWidth + uOffset, vOffset,
                    -0.5f, -0.5f, 0.0f,        0.0f + uOffset, vOffset
                    };
                const int floatsPerSprite = stride * vertsPerSprite;
                for (int j = 0; j < floatsPerSprite; ++j) {
                    int index = ((w * floatsPerSprite) + j) + (h * (floatsPerSprite * totalFramesWide));
                    vertices[index] = quad[j];
                }
            }
        }
        const int totalIndices = 6 * m_iTotalFrames;
        unsigned int *allIndices = new unsigned int[totalIndices];
        unsigned int i = 0;
        for (int k = 0; k < m_iTotalFrames; ++k) {
            unsigned int indices[] = {0 + i, 1 + i, 2 + i, 2 + i, 3 + i, 0 + i};
            for (int j = 0; j < 6; ++j) {
                int index = (k * 6) + j;
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

    void AnimatedSprite::Init() {
        SpriteNode::Init();
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

    void AnimatedSprite::Draw(Renderer& renderer) {
        //SpriteNode::Draw(renderer);
        SetupSpriteRendering(renderer);
        if (m_pSprite != nullptr) {
            SetupFrames();
            if (m_bAnimating) {
                assert(m_pVertexData);
                m_pSprite->SetActive();
                m_pVertexData->SetActive();
                renderer.DrawAnimatedSprite(*this->m_pSprite, m_iCurrentFrame, m_iFrameHeight, m_iFrameWidth);
            } else {
                assert(m_pVertexData);
                m_pSprite->SetActive();
                m_pVertexData->SetActive();
                renderer.DrawAnimatedSprite(*this->m_pSprite, 0, m_iFrameHeight, m_iFrameWidth);
            }
        }
    }

    void AnimatedSprite::Animate()
    {
        m_bAnimating = true;
    }
    void AnimatedSprite::StopAnimate()
    {
        m_bAnimating = false;
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

    void AnimatedSprite::DebugDraw()
    {
        ImGui::SliderInt("Frame ", &m_iCurrentFrame, 0, m_iTotalFrames - 1);
    }
}
