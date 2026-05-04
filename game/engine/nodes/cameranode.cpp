#include "cameranode.h"

#include "../../config/config.h"
#include "../../helper/inlinehelper.h"
#include "../physics/physicsmanager.h"


namespace Engine {
    CameraNode::CameraNode() {
        SetupNode("CameraNode", NT_CameraNode);
    }

    void CameraNode::Init() {
        Node::Init();
    }

    void CameraNode::Process(float deltaTime) {
        Node::Process(deltaTime);
    }

    void CameraNode::Draw(Renderer &renderer) {
        Node::Draw(renderer);

        renderer.SetOrthoViewport(Config::GetInstance().windowsWidth, Config::GetInstance().windowsHeight);
        renderer.SetOrthoOffset(m_globalTransform.position.x, m_globalTransform.position.y);
    }
}
