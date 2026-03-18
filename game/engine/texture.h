#ifndef TEXTURE_H
#define TEXTURE_H
#include <SDL_surface.h>
#include <vector>

namespace Engine {
    class Texture {
        // Member methods:
    public:
        Texture();
        ~Texture();

        bool Initialise(const char *pcFilename);
        void SetActive() const;
        int GetWidth() const;
        int GetHeight() const;

        void LoadTextTexture(const char *text, const char *fontname, int pointsize);

        void LoadSurfaceIntoTexture(SDL_Surface *pSurface);

    protected:
        unsigned int m_uiTextureId;
        int m_iWidth;
        int m_iHeight;

    private:
    };
}
#endif // TEXTURE_H