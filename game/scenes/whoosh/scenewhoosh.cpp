//
// Created by leon on 23.03.26.
//

#include "scenewhoosh.h"


SceneWhoosh::SceneWhoosh() : m_player(nullptr) {}

void SceneWhoosh::Init() {
    Node::Init();

    NodeFactory::GetInstance().InitWithConfiguration(this,"../game/scenes/whoosh/whoosh.ini");

    /*
    m_floor = new RigidbodyNode(b2_staticBody, 0, 0);
    m_floor->m_globalTransform.position = Vector2d(100,700);
    m_floor->m_globalTransform.SetSize(1000,10);
    m_floor->m_globalTransformationFlag = IF_Disable;
    AddChild(*m_floor);
    */


}
