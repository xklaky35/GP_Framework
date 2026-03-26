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


    b2BodyDef groundBodyDef = b2DefaultBodyDef();
    groundBodyDef.position = (b2Vec2){200.0f, 500.0f};
    b2BodyId groundId = b2CreateBody(PhysicsManager::GetInstance().GetWorld(), &groundBodyDef);
    b2Polygon groundBox = b2MakeBox(2000.0f, 10.0f);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

    LogManager::GetInstance().Log(INFO, "%f %f", b2Body_GetPosition(groundId).x ,b2Body_GetPosition(groundId).y);

}
