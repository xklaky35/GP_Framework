#include "texturemanager.h"
#include "texture.h"
#include "logmanager/logmanager.h"

#include <cassert>

namespace Engine {
    TextureManager::TextureManager() = default;
    TextureManager::~TextureManager() {
        std::map<std::string, Texture*>::iterator iter = m_pLoadedTextures.begin();
        while (iter != m_pLoadedTextures.end()) {
            Texture *pTexture = iter->second;
            delete pTexture;
            ++iter;
        }
        m_pLoadedTextures.clear();
    }

    bool TextureManager::Initialise
    () {
        LogManager::GetInstance().Log(INFO, "TextureManager starting...");
        return true;
    }

    Texture *TextureManager::GetTexture(const char *pcFilename) {
        Texture *pTexture = nullptr;
        if (m_pLoadedTextures.find(pcFilename) == m_pLoadedTextures.end()) {
            // Not already loaded... so load...
            pTexture = new Texture();
            if (!pTexture->Initialise(pcFilename)) {
                LogManager::GetInstance().Log(ERROR, "Texture failed to initialise!");
            }
            m_pLoadedTextures[pcFilename] = pTexture;
        } else {
            // It has already been loaded...
            pTexture = m_pLoadedTextures[pcFilename];
        }
        return pTexture;
    }
    void TextureManager::AddTexture(const char* key, Texture* pTexture)
    {
        m_pLoadedTextures[key] = pTexture;
    }
}
