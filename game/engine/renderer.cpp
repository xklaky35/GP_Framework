// Local includes:
#include "renderer.h"
#include "texturemanager.h"
#include "logmanager/logmanager.h"
#include "shader.h"
#include "sprite.h"
#include "structs/vertexarray.h"
#include "structs/matrix4.h"
// Library includes:

#include "SDL_image.h"
#include "GL/glew.h"
#include <cassert>
#include <cmath>

#include "../config/config.h"
#include "physics/physicsmanager.h"


namespace Engine {
    Renderer::Renderer()
        : m_pTextureManager(nullptr)
          , m_pSpriteShader(nullptr)
          , m_pSpriteVertexData(nullptr)
          , m_glContext(nullptr)
          , m_iWidth(0)
          , m_iHeight(0)
          , m_fClearRed(0.0f)
          , m_fClearGreen(0.0f)
          , m_fClearBlue(0.0f)
          , m_pWindow(nullptr) {

    }

    Renderer::~Renderer() {
        delete m_pSpriteShader;
        m_pSpriteShader = nullptr;
        delete m_pSpriteVertexData;
        m_pSpriteVertexData = nullptr;
        delete m_pTextureManager;
        m_pTextureManager = nullptr;
        SDL_DestroyWindow(m_pWindow);
        IMG_Quit();
        SDL_Quit();
    }


    bool Renderer::Initialise(const bool windowed, int width, int height) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            LogSdlError();
            return false;
        }
        if (!windowed) {
            // Go fullscreen, with current resolution!
            const int numDisplays = SDL_GetNumVideoDisplays();
            auto *currentDisplayMode = new SDL_DisplayMode[numDisplays];
            for (int k = 0; k < numDisplays; ++k) {
                int result = SDL_GetCurrentDisplayMode(k, &currentDisplayMode[k]);
            }
            // Use the widest display?
            int widest = 0;
            int andItsHeight = 0;
            for (int k = 0; k < numDisplays; ++k) {
                if (currentDisplayMode[k].w > widest) {
                    widest = currentDisplayMode[k].w;
                    andItsHeight = currentDisplayMode[k].h;
                }
            }
            delete[] currentDisplayMode;
            currentDisplayMode = nullptr;
            width = widest;
            height = andItsHeight;
        }
        bool initialised = InitialiseOpenGL(width, height);
        SetFullscreen(!windowed);
        if (initialised) {
            m_pTextureManager = new TextureManager();
            assert(m_pTextureManager);
            initialised = m_pTextureManager->Initialise();
        }

        CreateOrthoProjection(m_orthoProjection, static_cast<float>(width), static_cast<float>(height));
        return initialised;
    }

    bool Renderer::InitialiseOpenGL(const int screenWidth, const int screenHeight) {
        m_iWidth = screenWidth;
        m_iHeight = screenHeight;
        m_pWindow = SDL_CreateWindow("My own little game engine", SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        m_glContext = SDL_GL_CreateContext(m_pWindow);
        GLenum glewResult = glewInit();
        if (glewResult != GLEW_OK) {
            return false;
        }
        // Disable VSYNC
        SDL_GL_SetSwapInterval(0);
        bool shadersLoaded = SetupSpriteShader();


        // Scale to fit pixels to meters
        float lowerX = -25.0f, upperX = 25.0f, lowerY = -25.0f, upperY = 25.0f;
        gluOrtho2D(lowerX, upperX, lowerY, upperY);

        return shadersLoaded;

    }


    void Renderer::Clear() {
        glClearColor(m_fClearRed, m_fClearGreen, m_fClearBlue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::Present() {
        SDL_GL_SwapWindow(m_pWindow);
    }

    void Renderer::SetFullscreen(bool fullscreen) {
        if (fullscreen) {
            // SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_ALWAYS_ON_TOP);
            SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_ALWAYS_ON_TOP);
            SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
            SDL_SetWindowSize(m_pWindow, m_iWidth, m_iHeight);
        } else {
            SDL_SetWindowFullscreen(m_pWindow, 0);
        }
    }

    void Renderer::SetClearColour(unsigned char r, unsigned char g, unsigned char b) {
        m_fClearRed = r / 255.0f;
        m_fClearGreen = g / 255.0f;
        m_fClearBlue = b / 255.0f;
    }

    void Renderer::GetClearColour(unsigned char &r, unsigned char &g, unsigned char &b) {
        r = static_cast<unsigned char>(m_fClearRed * 255.0f);
        g = static_cast<unsigned char>(m_fClearGreen * 255.0f);
        b = static_cast<unsigned char>(m_fClearBlue * 255.0f);
    }

    int Renderer::GetWidth() const {
        return m_iWidth;
    }

    int Renderer::GetHeight() const {
        return m_iHeight;
    }

    Sprite *Renderer::CreateSprite(const char *pcFilename) {
        assert(m_pTextureManager);
        Texture *pTexture = m_pTextureManager->GetTexture(pcFilename);
        Sprite *pSprite = new Sprite();
        if (!pSprite->Initialise(*pTexture)) {
            LogManager::GetInstance().Log(ERROR, "Sprite Failed to Create!");
        }
        return (pSprite);
    }

    AnimatedSprite *Renderer::CreateAnimatedSprite(const char *pcFilename) {
        assert(m_pTextureManager);
        Texture *pTexture = m_pTextureManager->GetTexture(pcFilename);
        AnimatedSprite *pSprite = new AnimatedSprite();
        if (!pSprite->Initialise(*pTexture)) {
            LogManager::GetInstance().Log(ERROR, "Animated Sprite Failed to Create!");
        }
        return (pSprite);
    }

    void Renderer::LogSdlError() {
        LogManager::GetInstance().Log(ERROR, SDL_GetError());
    }


    bool Renderer::SetupSpriteShader() {
        m_pSpriteShader = new Shader();
        bool loaded = m_pSpriteShader->Load("shaders/sprite.vert", "shaders/sprite.frag");
        m_pSpriteShader->SetActive();
        float vertices[] =
        {
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Top left
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // Top right
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f // Bottom left
        };
        unsigned int indices[] = {0, 1, 2, 2, 3, 0};
        m_pSpriteVertexData = new VertexArray(vertices, 4, indices, 6);
        return loaded;
    }

    void Renderer::DrawSprite(Sprite &sprite) {
        m_renderList[sprite.m_iLayer].push_back(&sprite);
    }

    void Renderer::Draw() {
        for (int i = 0; i < m_renderList.size(); i++) {
            if (m_renderList.find(i) != m_renderList.end()) {
                for (auto* sprite : m_renderList[i]) {

                    float sizeX = 0;
                    float sizeY = 0;
                    int frameNumber = 0;
                    float offsetX = 0;
                    float offsetY = 0;

                    auto animSprite = dynamic_cast<AnimatedSprite *>(sprite);
                    if (animSprite) {
                        sizeX = animSprite->m_iFrameWidth * sprite->GetScale();
                        sizeY = animSprite->m_iFrameHeight * sprite->GetScale();
                        frameNumber = animSprite->m_iCurrentFrame;
                        animSprite->m_pVertexData->SetActive();
                        animSprite->SetActive();
                    }
                    else {

                        sizeX = static_cast<float>(sprite->GetWidth());
                        sizeY = static_cast<float>(sprite->GetHeight());
                        sprite->SetActive();
                        m_pSpriteVertexData->SetActive();
                    }

                    m_pSpriteShader->SetActive();

                    float angleInDegrees = sprite->GetAngle();
                    const float PI = 3.14159f;
                    float angleInRadians = (angleInDegrees * PI) / 180.0f;

                    Matrix4 world;
                    SetIdentity(world);
                    world.m[0][0] = cosf(angleInRadians) * (sizeX);
                    world.m[0][1] = -sinf(angleInRadians) * (sizeX);
                    world.m[1][0] = sinf(angleInRadians) * (sizeY);
                    world.m[1][1] = cosf(angleInRadians) * (sizeY);
                    world.m[3][0] = static_cast<float>(sprite->GetX());
                    world.m[3][1] = static_cast<float>(sprite->GetY());
                    m_pSpriteShader->SetMatrixUniform("uWorldTransform", world);
                    m_pSpriteShader->SetVector4Uniform("colour", sprite->GetRedTint(),
                                                       sprite->GetGreenTint(),
                                                       sprite->GetBlueTint(),
                                                       sprite->GetAlpha());
                    m_pSpriteShader->SetMatrixUniform("uViewProj", m_orthoProjection);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)((frameNumber * 6) * sizeof(GLuint)));
                }
                m_renderList[i].clear();
            }
        }

        m_renderList.clear();
    }

    void Renderer::DrawAnimatedSprite(Sprite &sprite, const int frame, const int frameHeight, const int frameWidth) {
        m_pSpriteShader->SetActive();

        float sizeX = static_cast<float>(frameWidth) * sprite.GetScale();
        float sizeY = static_cast<float>(frameHeight) * sprite.GetScale();

        float angleInDegrees = sprite.GetAngle();
        const float PI = 3.14159f;
        float angleInRadians = (angleInDegrees * PI) / 180.0f;

        Matrix4 world;
        SetIdentity(world);
        world.m[0][0] = cosf(angleInRadians) * (sizeX);
        world.m[0][1] = -sinf(angleInRadians) * (sizeX);
        world.m[1][0] = sinf(angleInRadians) * (sizeY);
        world.m[1][1] = cosf(angleInRadians) * (sizeY);
        world.m[3][0] = static_cast<float>(sprite.GetX());
        world.m[3][1] = static_cast<float>(sprite.GetY());

        m_pSpriteShader->SetMatrixUniform("uWorldTransform", world);

        m_pSpriteShader->SetVector4Uniform("colour", sprite.GetRedTint(),
                                           sprite.GetGreenTint(),
                                           sprite.GetBlueTint(),
                                           sprite.GetAlpha());
        m_pSpriteShader->SetMatrixUniform("uViewProj", m_orthoProjection);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)((frame * 6) * sizeof(GLuint)));
    }


    SDL_Window* Renderer::GetSDLWindow() {
        return m_pWindow;
    }

    const SDL_GLContext Renderer::GetSDLGLContext() {
       return m_glContext;
    }

    void Renderer::CreateStaticText(const char *pText, int pointsize, SDL_Color color) {
        Texture *pTexture = new Texture();
        pTexture->LoadTextTexture(pText, "../assets/Fonts/Romantic Mermaid.otf", pointsize, color);
        m_pTextureManager->AddTexture(pText, pTexture);
    }

    void Renderer::SetOrthoViewport(int width, int height) {
        m_orthoProjection.m[0][0] = 2.0f / (width);
        m_orthoProjection.m[1][1] = 2.0f / (-height);
    }

    void Renderer::SetOrthoOffset(float x, float y) {
        float offsetX =  m_orthoProjection.m[0][0] * x ;
        float offsetY = -m_orthoProjection.m[1][1] * y ;

        m_orthoProjection.m[3][0] = -1.f - offsetX + !((int)x == 0 && (int)y == 0);
        m_orthoProjection.m[3][1] =  1.f + offsetY - !((int)x == 0 && (int)y == 0);

        PhysicsManager::GetInstance().ChangeDebugOrthoPos(x, y);
    }
}
