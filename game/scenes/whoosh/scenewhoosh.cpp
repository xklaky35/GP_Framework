//
// Created by leon on 23.03.26.
//

#include "scenewhoosh.h"

SceneWhoosh::SceneWhoosh() : m_player(nullptr) {}

void SceneWhoosh::Init() {
    Node::Init();

    m_player = new Player();
    AddChild(*m_player);
}
