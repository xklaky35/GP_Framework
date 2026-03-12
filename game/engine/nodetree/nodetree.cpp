#include "nodetree.h"

#include "../../config/config.h"
#include "../../entities/spaceship.h"
#include "../../entities/SpinningRect.h"

namespace Engine {
    Nodetree *Nodetree::sm_pInstance = nullptr;

    Nodetree::Nodetree() {
        m_pRoot = new Node();
        m_pRoot->m_bIsRoot = true;
    }

    Nodetree::~Nodetree() {
        delete m_pRoot;
        m_pRoot = nullptr;
    }

    void Nodetree::Init() const {

        const int height = Config::GetInstance().windowsHeight;
        const int width = Config::GetInstance().windowsWidth;

        float rectWidth = height / 2 - 20;
        float rectHeight = height / 2 - 20;

        std::string spritePath = "../assets/Sprites/board8x8.png";

        Spritenode* rectTopLeft = new Spritenode(rectHeight, rectWidth,spritePath.c_str());
        rectTopLeft->m_bUseSpriteSize = false;
        rectTopLeft->m_position->x = rectWidth / 2;
        rectTopLeft->m_position->y = rectHeight / 2;
        m_pRoot->AddChild(*rectTopLeft);

        Spritenode* rectTopRight = new Spritenode(rectHeight, rectWidth,spritePath.c_str());
        rectTopRight->m_bUseSpriteSize = false;
        rectTopRight->m_position->x = width - (rectWidth / 2);
        rectTopRight->m_position->y = rectHeight / 2;
        rectTopRight->SetRGBA(1,0,0,1);
        m_pRoot->AddChild(*rectTopRight);

        Spritenode* rectBottomLeft = new Spritenode(rectHeight, rectWidth,spritePath.c_str());
        rectBottomLeft->m_bUseSpriteSize = false;
        rectBottomLeft->m_position->x = (rectWidth / 2);
        rectBottomLeft->m_position->y = height - rectHeight / 2;
        rectBottomLeft->SetRGBA(0,1,0,1);
        m_pRoot->AddChild(*rectBottomLeft);

        Spritenode* rectBottomRight = new Spritenode(rectHeight, rectWidth,spritePath.c_str());
        rectBottomRight->m_bUseSpriteSize = false;
        rectBottomRight->m_position->x = width - (rectWidth / 2);
        rectBottomRight->m_position->y = height - rectHeight / 2;
        rectBottomRight->SetRGBA(0,0,1,1);
        m_pRoot->AddChild(*rectBottomRight);

        SpinningRect* center = new SpinningRect(20.f,rectHeight, rectWidth,spritePath.c_str());
        center->m_bUseSpriteSize = false;
        center->m_position->x = width / 2;
        center->m_position->y = height / 2;
        m_pRoot->AddChild(*center);

        /*
        Spaceship *sp = new Spaceship();
        sp->m_position->x = 100;
        sp->m_position->y = 400;
        sp->velocity = 500;
        sp->speedFactor = 20;
        m_pRoot->AddChild(*sp);

        MetheorGenerator *gen = new MetheorGenerator();
        m_pRoot->AddChild(*gen);
        */
    }

    void Nodetree::Process(const float deltatime) const {
        m_pRoot->Process(deltatime);
    }

    void Nodetree::Draw(Renderer &renderer) const {
        m_pRoot->Draw(renderer);
    }

    Nodetree &Nodetree::GetInstance() {
        if (sm_pInstance == nullptr) {
            sm_pInstance = new Nodetree();
        }
        return (*sm_pInstance);
    }

    void Nodetree::DestroyInstance() {
        delete sm_pInstance;
        sm_pInstance = nullptr;
    }



    Node &Nodetree::GetRoot() const {
        return *m_pRoot;
    }
}
