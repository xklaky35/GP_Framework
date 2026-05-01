#include "cameranode.h"

#include "../../helper/inlinehelper.h"


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

    }
}
