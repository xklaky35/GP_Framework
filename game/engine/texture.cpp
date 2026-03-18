#include "texture.h"
#include "logmanager/logmanager.h"

#include <SDL_image.h>
#include <cassert>

#include "imgui.h"
#include "SDL_ttf.h"
#include "GL/glew.h"

namespace Engine {
    Texture::Texture()
        : m_uiTextureId(0)
          , m_iHeight(0)
          , m_iWidth(0) {
    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_uiTextureId);
    }

    bool Texture::Initialise(const char *pcFilename) {
        SDL_Surface *pSurface = IMG_Load(pcFilename);
        if (pSurface) {
            m_iWidth = pSurface->w;
            m_iHeight = pSurface->h;
            int bytesPerPixel = pSurface->format->BytesPerPixel;
            unsigned int format = 0;
            if (bytesPerPixel == 3) {
                format = GL_RGB;
            } else if (bytesPerPixel == 4) {
                format = GL_RGBA;
            }
            glGenTextures(1, &m_uiTextureId);
            glBindTexture(GL_TEXTURE_2D, m_uiTextureId);
            glTexImage2D(GL_TEXTURE_2D, 0, format, m_iWidth, m_iHeight, 0, format, GL_UNSIGNED_BYTE, pSurface->pixels);
            SDL_FreeSurface(pSurface);
            pSurface = nullptr;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        } else {
            LogManager::GetInstance().Log(ERROR, "Texture failed to load!");
            return false;
        }
        return true;
    }

    void Texture::SetActive() const {
        glBindTexture(GL_TEXTURE_2D, m_uiTextureId);
    }

    int Texture::GetWidth() const {
        assert(m_iWidth);
        return (m_iWidth);
    }

    int Texture::GetHeight() const {
        assert(m_iHeight);
        return (m_iHeight);
    }
    void Texture::LoadTextTexture(const char *text, const char *fontname, int pointsize) {
        TTF_Font *pFont = nullptr;
        TTF_Init();
        if (pFont == nullptr) {
            pFont = TTF_OpenFont(fontname, pointsize);
        }
        SDL_Color color;
        color.r = 0;
        color.g = 0;
        color.b = 0;
        color.a = 255;
        SDL_Surface *pSurface = TTF_RenderText_Blended(pFont, text, color);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, pSurface->pitch / pSurface->format->BytesPerPixel);
        LoadSurfaceIntoTexture(pSurface);
        TTF_CloseFont(pFont);
        pFont = nullptr;
    }

    void Texture::LoadSurfaceIntoTexture(SDL_Surface* pSurface)
    {
        if (pSurface)
        {
            m_iWidth = pSurface->w;
            m_iHeight = pSurface->h;
            int bytesPerPixel = pSurface->format->BytesPerPixel;
            unsigned int format = 0;
            if (bytesPerPixel == 3)
            {
                format = GL_RGB;
            }
            else if (bytesPerPixel == 4)
            {
                format = GL_RGBA;
            }
            glGenTextures(1, &m_uiTextureId);
            glBindTexture(GL_TEXTURE_2D, m_uiTextureId);
            glTexImage2D(GL_TEXTURE_2D, 0, format, m_iWidth, m_iHeight, 0, format,
            GL_UNSIGNED_BYTE, pSurface->pixels);
            SDL_FreeSurface(pSurface);
            pSurface = 0;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
    }

}
