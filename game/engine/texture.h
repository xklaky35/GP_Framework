#ifndef TEXTURE_H
#define TEXTURE_H
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


    protected:
        unsigned int m_uiTextureId;
        int m_iWidth;
        int m_iHeight;

    private:
    };
}
#endif // TEXTURE_H