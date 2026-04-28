//
// Created by leon on 23.03.26.
//

#include "scenewhoosh.h"

#include "../../engine/logmanager/logmanager.h"
#include "../../engine/physics/physicsmanager.h"
#include "box2d/box2d.h"

SceneWhoosh::SceneWhoosh() : m_player(nullptr) {}

void SceneWhoosh::Init() {
    Node::Init();

    m_player = new Player();
    AddChild(*m_player);


    m_floor = new RigidbodyNode(b2_staticBody, 0, 0);
    m_floor->m_globalTransform.position = Vector2d(100,700);
    m_floor->m_globalTransform.SetSize(1000,10);
    m_floor->m_globalTransformationFlag = IF_Disable;
    AddChild(*m_floor);


}
