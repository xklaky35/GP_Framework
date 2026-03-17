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
    class SpriteNode : public Node {


    public:
        SpriteNode();
        SpriteNode(const char* spritePath);
        SpriteNode(float height, float weight, const char* spritePath);
        ~SpriteNode() override;

        void Init() override;
        void Process(float deltaTime) override;
        void Draw(Renderer &) override;
        void DrawDebug() override;
        void SetRGBA(float,float,float,float);
        void SetSpritePath(const std::string &);


        SpriteDisplayFlag m_spriteDisplayMode;

    protected:

        void SetupSpriteRendering(Renderer&);


    protected:
        Sprite *m_pSprite;
    private:
        Renderer *m_pRenderer;
        std::string m_pSpritePath;
        Vector2d m_aspectRatio;
        float m_redTint;
        float m_greenTint;
        float m_blueTint;
        float m_alpha;
        float m_scaleFactor;
    };
}

#endif //GP_FRAMEWORK_SPRITENODE_H