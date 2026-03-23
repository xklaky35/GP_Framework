//
// Created by leon on 16.03.26.
//

#include "scenespaceinvader.h"

#include "../../../lib/BOX2D/include/box2d/box2d.h"
#include "../../../lib/BOX2D/include/box2d/types.h"
#include "../../config/config.h"
#include "../../engine/logmanager/logmanager.h"

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


    // world
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){0.0f, -10.0f};
    m_worldId = b2CreateWorld(&worldDef);


    // ground
    b2BodyDef groundBodyDef = b2DefaultBodyDef();
    groundBodyDef.position = (b2Vec2){0.0f, -10.0f};

    b2BodyId groundId = b2CreateBody(m_worldId, &groundBodyDef);
    b2Polygon groundBox = b2MakeBox(50.0f, 10.0f);

    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);



    // body
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = (b2Vec2){0.0f, 4.0f};
    m_bodyId = b2CreateBody(m_worldId, &bodyDef);

    b2Polygon dynamicBox = b2MakeBox(1.0f, 1.0f);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.material.friction = 0.3f;
    b2CreatePolygonShape(m_bodyId, &shapeDef, &dynamicBox);


}
