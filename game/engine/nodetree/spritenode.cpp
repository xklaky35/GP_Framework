#include "spritenode.h"

namespace Engine {
    Spritenode::Spritenode(const char *nodeName) : Node(nodeName), mp_renderer(nullptr), mp_sprite(nullptr) {}
    Spritenode::~Spritenode() = default;

    void Spritenode::Init() {
        Node::Init();
    }

    void Spritenode::Process(float deltaTime) {
        Node::Process(deltaTime);
        if (mp_sprite != nullptr) {
            mp_sprite->SetX(m_position->x);
            mp_sprite->SetY(m_position->y);

            mp_sprite->SetScale(m_transform->scale);
            mp_sprite->SetAngle(m_transform->rotation);
        }
    }

    void Spritenode::Draw(Renderer &renderer) {
        Node::Draw(renderer);

        SetRenderer(renderer);

        if (mp_sprite != nullptr) {
            renderer.DrawSprite(*mp_sprite);
        }
    }

    void Spritenode::SetRenderer(Renderer &renderer) {
        if (mp_renderer == nullptr) {
            mp_renderer = &renderer;
        }
        if (mp_sprite == nullptr) {
            mp_sprite = mp_renderer->CreateSprite(mp_spritePath.c_str());
        }
    }

    void Spritenode::SetSprite(const std::string &path) {
        mp_spritePath = path;
    }
}
