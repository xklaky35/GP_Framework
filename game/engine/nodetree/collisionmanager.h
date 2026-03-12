#ifndef GP_FRAMEWORK_COLLISIONMANAGER_H
#define GP_FRAMEWORK_COLLISIONMANAGER_H

#include <vector>

#include "collidernode.h"

namespace Engine {

    class CollisionManager {
        // member functions
    public:
        static CollisionManager &GetInstance();
        static void DestroyInstane();

        ColliderNode *CheckForCollision(const ColliderNode &) const;
        void RegisterCollider(ColliderNode&);

    private:
        CollisionManager();
        ~CollisionManager();

        // member variables
    protected:
        static CollisionManager *sm_pInstance;

    private:
        std::vector<ColliderNode *> colliders;
    };
}
#endif //GP_FRAMEWORK_COLLISIONMANAGER_H