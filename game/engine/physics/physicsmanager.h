#ifndef GP_FRAMEWORK_PHYSICSMANAGER_H
#define GP_FRAMEWORK_PHYSICSMANAGER_H
#include "../structs/vector2d.h"
#include "box2d/id.h"


namespace Engine {
    class PhysicsManager {
    public:
        static PhysicsManager &GetInstance();
        static void DestroyInstance();

        bool Initialise();
        void Process(float deltaTime);
        b2WorldId GetWorld();

        static float PixelsToMeter(float pixels);
        static float MeterToPixels(float meter);
        static Vector2d PixelsToMeterVector(Vector2d pixels);
        static Vector2d MeterToPixelsVector(Vector2d meter);

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