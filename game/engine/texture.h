#ifndef TEXTURE_H
#define TEXTURE_H
#include <SDL_surface.h>

namespace Engine {
    class Texture {
        // Member methods:
    public:
        Texture();
        ~Texture();

        bool Initialise(const char *pcFilename);
        void SetActive() const;
        [[nodiscard]] int GetWidth() const;
        [[nodiscard]] int GetHeight() const;

        void LoadTextTexture(const char *text, const char *fontname, int pointsize, SDL_Color);

        void LoadSurfaceIntoTexture(SDL_Surface *pSurface);

    protected:
        unsigned int m_uiTextureId;
        int m_iWidth;
        int m_iHeight;

    private:
    };
}
#endif // TEXTURE_H