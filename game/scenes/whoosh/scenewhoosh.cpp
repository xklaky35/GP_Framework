//
// Created by leon on 23.03.26.
//

#include "scenewhoosh.h"


SceneWhoosh::SceneWhoosh()
    : m_pPlayer(nullptr),
      m_pFloor(nullptr) {}

void SceneWhoosh::Init() {
    Node::Init();

    m_name = "Root";
    NodeFactory::GetInstance().InitWithConfiguration(this,"../game/scenes/whoosh/Whoosh.ini");
}
