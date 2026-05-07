#include "physicsmanager.h"

#include "debugdraw.h"
#include "../../config/config.h"
#include "../logmanager/logmanager.h"
#include "../structs/vector2d.h"
#include "box2d/box2d.h"
#include "box2d/types.h"
#include "GL/glew.h"

namespace Engine {
    class Renderer;

    static constexpr float kPixelsToMeters = 0.02f;  // 50px per meter
    static constexpr float kMetersToPixels = 50.0f;

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

        m_debugDraw.Initialise(0, Config::GetInstance().windowsWidth * kPixelsToMeters,Config::GetInstance().windowsHeight * kPixelsToMeters, 0);
        m_b2DebugDraw = m_debugDraw.BuildDebugDraw();

        return true;
    }

    void PhysicsManager::Process(float deltaTime) {
        b2World_Step(m_gameWorldId, deltaTime, 4);
    }

    void PhysicsManager::DrawDebug() {
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        b2World_Draw( m_gameWorldId, &m_b2DebugDraw );
    }

    b2WorldId PhysicsManager::GetWorld() {
        return m_gameWorldId;
    }

    void PhysicsManager::ResetGameWorld() {
        b2DestroyWorld(m_gameWorldId);
        Initialise();
    }

    void PhysicsManager::ChangeDebugOrthoPos(float x, float y) {
        DebugDraw::MakeOrtho(m_debugDraw.m_proj, 0, Config::GetInstance().windowsWidth * kPixelsToMeters,Config::GetInstance().windowsHeight * kPixelsToMeters, 0, x,y);
    }

    // 50 pixels per meter
    float PhysicsManager::PixelsToMeter(float pixels) {
        return kPixelsToMeters * pixels;
    }

    float PhysicsManager::MeterToPixels(float meter) {
        return kMetersToPixels * meter;
    }

    Vector2d PhysicsManager::PixelsToMeterVector(Vector2d pixels) {
        return {
            pixels.x * kPixelsToMeters,
            pixels.y * kPixelsToMeters
        };
    }

    Vector2d PhysicsManager::MeterToPixelsVector(Vector2d meters) {
        return {
            meters.x * kMetersToPixels,
            meters.y * kMetersToPixels
        };
    }




    PhysicsManager::PhysicsManager() : m_gameWorldId() {

    } ;
    PhysicsManager::~PhysicsManager() {
        b2DestroyWorld(m_gameWorldId);
    }
}
