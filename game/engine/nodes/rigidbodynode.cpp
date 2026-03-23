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
    }

    RigidbodyNode::~RigidbodyNode() {

    }

    void RigidbodyNode::Init() {
        Node::Init();


        b2BodyDef groundBodyDef = b2DefaultBodyDef();
        groundBodyDef.position = (b2Vec2){m_parent->m_globalTransform.position.x, m_parent->m_globalTransform.position.y};
        m_bodyId = b2CreateBody(PhysicsManager::GetInstance().GetWorld(), &groundBodyDef);


        b2Polygon dynamicBox = b2MakeBox(1.0f, 1.0f);
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        //shapeDef.material.friction = 0.3f;
        b2CreatePolygonShape(m_bodyId, &shapeDef, &dynamicBox);

        m_parent->m_globalTransformationFlag = Disable;

        b2Body_SetType(m_bodyId, m_bodyType);
    }

    void RigidbodyNode::Process(float deltaTime) {
        Node::Process(deltaTime);

        assert(m_parent->m_globalTransformationFlag == Disable);

        Vector2d newVec = Vector2d(b2Body_GetLinearVelocity(m_bodyId).x, b2Body_GetLinearVelocity(m_bodyId).y) * deltaTime ;

        m_parent->m_globalTransform.position = m_parent->m_globalTransform.position * newVec;
    }
}

