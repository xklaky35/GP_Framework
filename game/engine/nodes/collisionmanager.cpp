#include "collisionmanager.h"

#include "../logmanager/logmanager.h"

namespace Engine {
    CollisionManager *CollisionManager::sm_pInstance = nullptr;

    CollisionManager::CollisionManager() = default;
    CollisionManager::~CollisionManager() = default;

    CollisionManager &CollisionManager::GetInstance() {
        if (sm_pInstance == nullptr) {
            sm_pInstance = new CollisionManager();
        }
        return *sm_pInstance;
    }

    void CollisionManager::DestroyInstane() {
        delete sm_pInstance;
        sm_pInstance = nullptr;
    }

    void CollisionManager::RegisterCollider(ColliderNode&col) {
        colliders.push_back(&col);
        LogManager::GetInstance().Log(INFO, "Collider registered: %s", col.name.c_str());
    }

    ColliderNode *CollisionManager::CheckForCollision(const ColliderNode &col) const {
        for (ColliderNode *c: colliders) {

            if (&col == c) continue;
            if (col.GetForm()->Overlap(*(c->GetForm()))) {
                return c;
            }
        }
        return nullptr;
    }
}
