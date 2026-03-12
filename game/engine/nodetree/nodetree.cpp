#include "nodetree.h"

#include "../../entities/spaceship.h"
#include "../../entities/metheorgenerator.h"

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
