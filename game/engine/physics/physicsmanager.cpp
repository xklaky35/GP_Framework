#include "physicsmanager.h"

#include "../structs/vector2d.h"
#include "box2d/box2d.h"
#include "box2d/types.h"

namespace Engine {

    PhysicsManager* PhysicsManager::m_pInstance = nullptr;

    PhysicsManager & PhysicsManager::GetInstance() {
        if (m_pInstance == nullptr) {
            m_pInstance = new PhysicsManager();
        }
        return *m_pInstance;
    }

    void PhysicsManager::DestroyInstance() {

        delete m_pInstance;
        m_pInstance = nullptr;
    }

    bool PhysicsManager::Initialise() {
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = (b2Vec2){0.0f, 9.8f};
        m_gameWorldId = b2CreateWorld(&worldDef);
        return true;
    }

    void PhysicsManager::Process(float deltaTime) {
        b2World_Step(m_gameWorldId, deltaTime, 4);
    }

    b2WorldId PhysicsManager::GetWorld() {
        return m_gameWorldId;
    }


    PhysicsManager::PhysicsManager() = default;
    PhysicsManager::~PhysicsManager() {
        b2DestroyWorld(m_gameWorldId);
    }
}
