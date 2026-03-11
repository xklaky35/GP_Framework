#ifndef GP_FRAMEWORK_NODETREE_H
#define GP_FRAMEWORK_NODETREE_H

#include "node.h"

namespace Engine {
    class Nodetree {
    public:
        static Nodetree &GetInstance();
        static void DestroyInstance();

        void Init() const;
        void Process(float) const;
        void Draw(Renderer &) const;

        Node& GetRoot() const;

    private:
        Nodetree();
        ~Nodetree();

    public:
        Node *m_pRoot;

    protected:
        static Nodetree *sm_pInstance;
    };
}

#endif //GP_FRAMEWORK_NODETREE_H