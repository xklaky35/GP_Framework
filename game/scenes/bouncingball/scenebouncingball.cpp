#include "scenebouncingball.h"

#include "bouncingball.h"

using namespace Engine;

SceneBouncingBall::SceneBouncingBall() : m_iBallCount(100) {}
SceneBouncingBall::~SceneBouncingBall() = default;

void SceneBouncingBall::Init() {
    Node::Init();

    m_bIsRoot = true;
    CreateBalls();
}

void SceneBouncingBall::Process(float deltaTime) {
    Node::Process(deltaTime);
}

void SceneBouncingBall::Draw(Renderer &renderer) {
    Node::Draw(renderer);
}

void SceneBouncingBall::CreateBalls() {
    for (int i = 0; i < m_iBallCount; i++) {
        AddChild(*new BouncingBall());
    }
}
