#include "cameranode.h"

#include "../../config/config.h"
#include "../physics/physicsmanager.h"

namespace Engine {
    CameraNode::CameraNode() {
        m_pRenderer = nullptr;
        SetupNode("CameraNode", NT_CameraNode);
    }

    CameraNode::~CameraNode() {
        if (m_pRenderer != nullptr) {
            m_pRenderer->SetOrthoOffset(0,0);
        }
    }

    void CameraNode::Init() {
        Node::Init();
    }

    void CameraNode::Process(float deltaTime) {
        Node::Process(deltaTime);
    }

    void CameraNode::Draw(Renderer &renderer) {
        Node::Draw(renderer);

        if (m_pRenderer == nullptr) {
            m_pRenderer = &renderer;
        }

        renderer.SetOrthoOffset(m_globalTransform.position.x, m_globalTransform.position.y);
    }
}
