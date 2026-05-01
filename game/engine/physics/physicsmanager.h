#ifndef GP_FRAMEWORK_PHYSICSMANAGER_H
#define GP_FRAMEWORK_PHYSICSMANAGER_H
#include "debugdraw.h"
#include "../../../lib/BOX2D/src/joint.h"
#include "../structs/vector2d.h"
#include "box2d/id.h"


namespace Engine {
    class Renderer;

    class PhysicsManager {
    public:
        static PhysicsManager &GetInstance();
        static void DestroyInstance();

        bool Initialise();
        void Process(float deltaTime);
        void DrawDebug();
        b2WorldId GetWorld();

        void ResetGameWorld();

        static float PixelsToMeter(float pixels);
        static float MeterToPixels(float meter);
        static Vector2d PixelsToMeterVector(Vector2d pixels);
        static Vector2d MeterToPixelsVector(Vector2d meter);

    protected:
        static PhysicsManager* m_pInstance;

    private:

        PhysicsManager();
        ~PhysicsManager();

        b2WorldId m_gameWorldId;
        b2DebugDraw m_b2DebugDraw;
        DebugDraw m_debugDraw;
    };

}

#endif //GP_FRAMEWORK_PHYSICSMANAGER_H