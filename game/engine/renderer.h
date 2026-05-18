#ifndef RENDERER_H
#define RENDERER_H

#include "structs/vertexarray.h"
#include "shader.h"
#include "texturemanager.h"
#include "sprite.h"
#include "animatedsprite.h"


// Library includes:
#include <SDL.h>
#include <vector>

#include "structs/vector2d.h"


namespace Engine {
    class Renderer {
        // Member methods:
    public:
        Renderer();
        ~Renderer();

        bool Initialise(bool windowed, int width = 0, int height = 0);
        void Clear();
        void Present();


        Sprite *CreateSprite(const char *pcFilename);
        AnimatedSprite *CreateAnimatedSprite(const char *pcFilename);

        void DrawSprite(Sprite &sprite);
        void Draw(); // draw all sprites collected

        void CreateStaticText(const char *pText, int pointsize, SDL_Color) const;

        void GetClearColour(unsigned char &r, unsigned char &g, unsigned char &b) const;
        [[nodiscard]] Vector2d GetCurrentOffset() const;
        [[nodiscard]] SDL_Window* GetSDLWindow() const;
        [[nodiscard]] SDL_GLContext GetSDLGLContext() const;
        [[nodiscard]] int GetWidth() const;
        [[nodiscard]] int GetHeight() const;

        void SetClearColour(unsigned char r, unsigned char g, unsigned char b);
        void SetOrthoViewport(int windows_width, int windows_height);
        void SetOrthoOffset(float x, float y);


    protected:
        bool InitialiseOpenGL(int screenWidth, int screenHeight);
        void SetFullscreen(bool fullscreen);
        void LogSdlError();
        bool SetupSpriteShader();

    private:
        Renderer(const Renderer &renderer);
        Renderer &operator=(const Renderer &renderer);

        // Member data:
    public:
    protected:
        Matrix4 m_orthoProjection;
        std::map<int, std::vector<Sprite*>> m_renderList;
        TextureManager *m_pTextureManager;
        SDL_Window *m_pWindow;
        SDL_GLContext m_glContext;
        Shader *m_pSpriteShader;
        VertexArray *m_pSpriteVertexData;
        int m_iWidth;
        int m_iHeight;
        float m_fClearRed;
        float m_fClearGreen;
        float m_fClearBlue;
        Vector2d m_currentOffset;

    };
}
#endif // RENDERER_H
