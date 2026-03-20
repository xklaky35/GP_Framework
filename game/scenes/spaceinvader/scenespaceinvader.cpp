//
// Created by leon on 16.03.26.
//

#include "scenespaceinvader.h"

#include "../../config/config.h"

using namespace Engine;

SceneSpaceinvader::SceneSpaceinvader() = default;


void SceneSpaceinvader::Init() {
    Node::Init();

    m_metheorGenerator = new MeteorGenerator(20);
    AddChild(*m_metheorGenerator);

    m_spaceship = new Spaceship(200);
    m_spaceship->m_globalTransform.position.x = Config::GetInstance().windowsWidth / 2;
    m_spaceship->m_globalTransform.position.y = Config::GetInstance().windowsHeight - Config::GetInstance().windowsHeight / 4;
    AddChild(*m_spaceship);
}
