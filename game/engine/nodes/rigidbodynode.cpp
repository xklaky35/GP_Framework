//
// Created by leon on 23.03.26.
//

#include "rigidbodynode.h"
#include <cassert>

#include "../logmanager/logmanager.h"
#include "../physics/physicsmanager.h"
#include "box2d/box2d.h"
#include "box2d/types.h"

namespace Engine {
    RigidbodyNode::RigidbodyNode(b2BodyType type) : m_bodyId(), m_bodyType(type), m_bIsStatic(true) {
        m_bodyPolygon = b2MakeBox(800.0f, 10.0f);
        m_iniParser = new IniParser();
    }

    void RigidbodyNode::Init() {
        Node::Init();

        // definition
        b2BodyDef groundBodyDef = b2DefaultBodyDef();
        groundBodyDef.position = (b2Vec2){m_parent->m_globalTransform.position.x, m_parent->m_globalTransform.position.y};

        // create body with definition
        m_bodyId = b2CreateBody(PhysicsManager::GetInstance().GetWorld(), &groundBodyDef);

        // create a shape definition
        b2ShapeDef shapeDef = b2DefaultShapeDef();

        // create shape with definition
        b2CreatePolygonShape(m_bodyId, &shapeDef, &m_bodyPolygon);

        m_parent->m_globalTransformationFlag = Disable;
        b2Body_SetType(m_bodyId, m_bodyType);
    }

    void RigidbodyNode::Process(float deltaTime) {
        Node::Process(deltaTime);

        // this node should not be used as base node
        assert(m_parent->m_globalTransformationFlag == Disable);

        // apply gravity to parent node only if transfrom can be changed
        Vector2d newVec = Vector2d(b2Body_GetLinearVelocity(m_bodyId).x, b2Body_GetLinearVelocity(m_bodyId).y);

        m_parent->m_globalTransform.position = m_parent->m_globalTransform.position + (newVec * deltaTime);
        LogManager::GetInstance().Log(INFO, "%f %f", b2Body_GetLinearVelocity(m_bodyId).x ,b2Body_GetLinearVelocity(m_bodyId).y);
    }


    void RigidbodyNode::AddForceToCenter(Vector2d vec) {
        b2Body_ApplyLinearImpulseToCenter(m_bodyId, b2Vec2(vec.x, vec.y), true);
    }
}

