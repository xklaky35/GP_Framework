#ifndef RENDERER_H
#define RENDERER_H

#include "structs/vertexarray.h"
#include "shader.h"
#include "texturemanager.h"
#include "sprite.h"


// Library includes:
#include <SDL.h>



namespace Engine {
    class Renderer {
        // Member methods:
    public:
        Renderer();
        ~Renderer();

        bool Initialise(bool windowed, int width = 0, int height = 0);
        void Clear();
        void Present();
        void SetClearColour(unsigned char r, unsigned char g, unsigned char b);
        void GetClearColour(unsigned char &r, unsigned char &g, unsigned char &b);
        int GetWidth() const;
        int GetHeight() const;


        Sprite *CreateSprite(const char *pcFilename);
        void DrawSprite(Sprite &sprite);
        void DrawAnimatedSprite(const Sprite &sprite, int frame, int width, int height);
        void Draw(); // draw all sprites collected
        SDL_Window* GetSDLWindow();
        const SDL_GLContext GetSDLGLContext();

        void CreateStaticText(const char *pText, int pointsize, SDL_Color);

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

    private:
    };
}
#endif // RENDERER_H
