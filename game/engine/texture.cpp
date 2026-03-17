#include "texture.h"
#include "logmanager/logmanager.h"

#include <SDL_image.h>
#include <cassert>

#include "imgui.h"
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
            glTexImage2D(GL_TEXTURE_2D, 0, format, m_iWidth, m_iHeight, 0,
                         format, GL_UNSIGNED_BYTE, pSurface->pixels);
            SDL_FreeSurface(pSurface);
            pSurface = 0;
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

}
