#include "physicsmanager.h"

#include "debugdraw.h"
#include "../../config/config.h"
#include "../structs/vector2d.h"
#include "box2d/box2d.h"
#include "box2d/types.h"
#include "GL/glew.h"

namespace Engine {
    class Renderer;

    static constexpr float s_fPixelsToMeters = 0.02f;  // 50px per meter
    static constexpr float s_fMetersToPixels = 50.0f;

    PhysicsManager* PhysicsManager::s_pInstance = nullptr;

    PhysicsManager & PhysicsManager::GetInstance() {
        if (s_pInstance == nullptr) {
            s_pInstance = new PhysicsManager();
        }
        return *s_pInstance;
    }

    void PhysicsManager::DestroyInstance() {

        delete s_pInstance;
        s_pInstance = nullptr;
    }

    bool PhysicsManager::Initialise() {
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = (b2Vec2){0.0f, 9.8f};
        m_gameWorldId = b2CreateWorld(&worldDef);

        m_debugDraw.Initialise(0, static_cast<float>(Config::GetInstance().windowsWidth) * s_fPixelsToMeters,static_cast<float>(Config::GetInstance().windowsHeight) * s_fPixelsToMeters, 0);
        m_box2DebugDraw = m_debugDraw.BuildDebugDraw();

        return true;
    }

    void PhysicsManager::Process(float deltaTime) {
        b2World_Step(m_gameWorldId, deltaTime, 4);
    }

    void PhysicsManager::DrawDebug() {
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        b2World_Draw( m_gameWorldId, &m_box2DebugDraw );
    }

    b2WorldId PhysicsManager::GetWorld() {
        return m_gameWorldId;
    }

    void PhysicsManager::ResetGameWorld() {
        b2DestroyWorld(m_gameWorldId);
        Initialise();
    }

    void PhysicsManager::ChangeDebugOrthoPos(float x, float y) {
        DebugDraw::MakeOrtho(m_debugDraw.m_proj, 0, static_cast<float>(Config::GetInstance().windowsWidth) * s_fPixelsToMeters,static_cast<float>(Config::GetInstance().windowsHeight) * s_fPixelsToMeters, 0, x,y);
    }

    // 50 pixels per meter
    float PhysicsManager::PixelsToMeter(float pixels) {
        return s_fPixelsToMeters * pixels;
    }

    float PhysicsManager::MeterToPixels(float meter) {
        return s_fMetersToPixels * meter;
    }

    Vector2d PhysicsManager::PixelsToMeterVector(Vector2d pixels) {
        return {
            pixels.x * s_fPixelsToMeters,
            pixels.y * s_fPixelsToMeters
        };
    }

    Vector2d PhysicsManager::MeterToPixelsVector(Vector2d meters) {
        return {
            meters.x * s_fMetersToPixels,
            meters.y * s_fMetersToPixels
        };
    }


    PhysicsManager::PhysicsManager() : m_gameWorldId(), m_box2DebugDraw() {} ;
    PhysicsManager::~PhysicsManager() {
        b2DestroyWorld(m_gameWorldId);
    }
}
