#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <string>
#include <map>

#include "texture.h"

namespace Engine {
    class TextureManager {
        // Member methods:
    public:
        TextureManager();
        ~TextureManager();

        bool Initialise();
        Texture *GetTexture(const char *pcFilename);

        void AddTexture(const char *key, Texture *pTexture);

    protected:
        std::map<std::string, Texture *> m_pLoadedTextures;
    };
}
#endif // TEXTUREMANAGER_H
