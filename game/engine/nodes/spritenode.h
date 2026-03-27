#ifndef GP_FRAMEWORK_SPRITENODE_H
#define GP_FRAMEWORK_SPRITENODE_H

#include "node.h"
#include "../renderer.h"

namespace Engine {
    enum SpriteDisplayFlag {
        Original, // use original sprite size
        Scale,  // chains width and hight of original sprite to keep dimensions
        Fit     // fit the specified hight and width values
    };
    static const char* SpriteDisplayFlagStrings[] = { "Original", "Scale", "Fit"};

    class SpriteNode : public Node {

    public:
        SpriteNode();
        SpriteNode(IniParser* iniParser);

        void Init() override;
        void SystemProcess() override;
        void Draw(Renderer &) override;
        void DrawDebug() override;
        void SetRGBA(float,float,float,float);
        void SetSpritePath(std::string);


    protected:

        virtual void SetupSpriteRendering(Renderer&);


    public:
        std::string m_spritePath;
        int m_iLayer;
        SpriteDisplayFlag m_spriteDisplayMode;

    protected:
        Sprite *m_pSprite;
        Renderer *m_pRenderer;
        Vector2d m_aspectRatio;
        float m_redTint;
        float m_greenTint;
        float m_blueTint;
        float m_alpha;

        float m_scaleFactor;
    };
}

#endif //GP_FRAMEWORK_SPRITENODE_H