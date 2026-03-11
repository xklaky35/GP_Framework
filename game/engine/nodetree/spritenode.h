#ifndef GP_FRAMEWORK_SPRITENODE_H
#define GP_FRAMEWORK_SPRITENODE_H

#include "node.h"
#include "../renderer.h"

namespace Engine {
    class Spritenode : public Node {
    public:
        Spritenode(const char* = "Sprite");
        ~Spritenode() override;

        void SetSprite(const std::string &);
        void Init() override;
        void Process(float deltaTime) override;
        void Draw(Renderer &) override;

        void SetRenderer(Renderer &);

    private:
        Renderer *mp_renderer;
        Sprite *mp_sprite;
        std::string mp_spritePath;
    };
}

#endif //GP_FRAMEWORK_SPRITENODE_H