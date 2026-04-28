#include "rigidbodynode.h"
#include <cassert>

#include "../logmanager/logmanager.h"
#include "../physics/physicsmanager.h"
#include "box2d/box2d.h"
#include "box2d/types.h"

namespace Engine {
    RigidbodyNode::RigidbodyNode(b2BodyType type, float mass, float friction) : m_bodyId(), m_bodyType(type), m_fMass(mass), m_bIsActive(false),
                                                    m_bIsSleeping(false),
                                                    m_bHasFixedRotation(false),
                                                    m_bIsBullet(false), m_shapeId(), m_fFriction(friction),
                                                    m_bIsStatic(true) {
        m_name = "RigidBodyNode";
        m_nodeType = NT_RigidBody;
    }

    RigidbodyNode::RigidbodyNode() : RigidbodyNode(b2_dynamicBody, 0,0) {}

    void RigidbodyNode::Init() {
        Node::Init();

        // definition
        b2BodyDef groundBodyDef = b2DefaultBodyDef();
        groundBodyDef.position = (b2Vec2){PhysicsManager::PixelsToMeter(m_globalTransform.position.x), PhysicsManager::PixelsToMeter(m_globalTransform.position.y)};
        groundBodyDef.userData = m_parent;

        // create body with definition
        m_bodyId = b2CreateBody(PhysicsManager::GetInstance().GetWorld(), &groundBodyDef);
        SetMassData(m_fMass, Vector2d(0,0), 0);

        // create a shape definition
        b2ShapeDef shapeDef = b2DefaultShapeDef();

        // create shape with definition
        m_bodyPolygon = b2MakeBox(PhysicsManager::PixelsToMeter(m_globalTransform.GetWidth() > 0 ? m_globalTransform.GetWidth() : 1), PhysicsManager::PixelsToMeter(m_globalTransform.GetHeight() > 0 ? m_globalTransform.GetHeight() : 1));
        m_shapeId = b2CreatePolygonShape(m_bodyId, &shapeDef, &m_bodyPolygon);

        m_parent->m_globalTransformationFlag = IF_Disable;
        b2Body_SetType(m_bodyId, m_bodyType);
    }

    void RigidbodyNode::Process(float deltaTime) {
        Node::Process(deltaTime);
    }

    // in pixels
    Vector2d RigidbodyNode::GetBodyPosition() {

        return PhysicsManager::MeterToPixelsVector({b2Body_GetPosition(m_bodyId).x, b2Body_GetPosition(m_bodyId).y});
    }

    b2Rot RigidbodyNode::GetBodyRotation() {

        return b2Body_GetRotation(m_bodyId);
    }

    Vector2d RigidbodyNode::GetBodyVelocity() {

        return {b2Body_GetLinearVelocity(m_bodyId).x, b2Body_GetLinearVelocity(m_bodyId).y};
    }

    void RigidbodyNode::ApplyForceToCenter(Vector2d vec) const {
        b2Body_ApplyForceToCenter(m_bodyId, b2Vec2{vec.x, vec.y}, true);
    }


    void RigidbodyNode::ApplyImpluseToCenter(Vector2d vec) {
        b2Body_ApplyLinearImpulseToCenter(m_bodyId, b2Vec2(vec.x, vec.y), true);
    }

    void RigidbodyNode::ResetBody() {
        b2Body_SetLinearVelocity(m_bodyId, b2Vec2(0,0));
    }

    void RigidbodyNode::SetMassData(float mass, Vector2d massCenter, float rotationalInertia) {
        auto newMassData = b2MassData();
        newMassData.mass = mass;
        newMassData.center = b2Vec2(massCenter.x, massCenter.y);
        newMassData.rotationalInertia = rotationalInertia;
        b2Body_SetMassData(m_bodyId, newMassData);
    }

    b2MassData RigidbodyNode::GetMassData() {
        return b2Body_GetMassData(m_bodyId);
    }


    void RigidbodyNode::SetVelocity(Vector2d velocity) {
        b2Body_SetLinearVelocity(m_bodyId, b2Vec2(velocity.x, velocity.y));
    }


    void RigidbodyNode::SetFriction(float friction) {
        b2Shape_SetFriction(m_shapeId, friction);
    }

    float RigidbodyNode::GetFriction() {
        return b2Shape_GetFriction(m_shapeId);
    }
}

