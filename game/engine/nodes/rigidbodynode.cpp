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
        m_bodyPolygon = b2MakeBox(1.0f, 1.0f);
    }

    RigidbodyNode::~RigidbodyNode() {

    }

    void RigidbodyNode::Init() {
        Node::Init();


        b2BodyDef groundBodyDef = b2DefaultBodyDef();
        groundBodyDef.position = (b2Vec2){m_parent->m_globalTransform.position.x, m_parent->m_globalTransform.position.y};
        m_bodyId = b2CreateBody(PhysicsManager::GetInstance().GetWorld(), &groundBodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        b2CreatePolygonShape(m_bodyId, &shapeDef, &m_bodyPolygon);

        m_parent->m_globalTransformationFlag = Disable;

        b2Body_SetType(m_bodyId, m_bodyType);
    }

    void RigidbodyNode::Process(float deltaTime) {
        Node::Process(deltaTime);

        assert(m_parent->m_globalTransformationFlag == Disable);

        Vector2d newVec = Vector2d(b2Body_GetLinearVelocity(m_bodyId).x, b2Body_GetLinearVelocity(m_bodyId).y);

        m_parent->m_globalTransform.position = m_parent->m_globalTransform.position + (newVec * deltaTime) * 100;
    }

    void RigidbodyNode::AddForceToCenter(Vector2d vec) {
        b2Body_ApplyLinearImpulseToCenter(m_bodyId, b2Vec2(vec.x, vec.y), true);
    }
}

