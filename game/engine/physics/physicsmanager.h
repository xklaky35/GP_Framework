#ifndef GP_FRAMEWORK_PHYSICSMANAGER_H
#define GP_FRAMEWORK_PHYSICSMANAGER_H
#include "box2d/id.h"


namespace Engine {
    class PhysicsManager {
    public:
        static PhysicsManager &GetInstance();
        static void DestroyInstance();

        bool Initialise();
        void Process(float deltaTime);
        b2WorldId GetWorld();
        void RegisterObject();

    protected:
        static PhysicsManager* m_pInstance;

    private:
        PhysicsManager();
        ~PhysicsManager();

        b2WorldId m_gameWorldId;
    };

}

#endif //GP_FRAMEWORK_PHYSICSMANAGER_H