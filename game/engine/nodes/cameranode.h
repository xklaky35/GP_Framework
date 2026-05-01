#ifndef GP_FRAMEWORK_CAMERANODE_H
#define GP_FRAMEWORK_CAMERANODE_H
#include "nodefactory.h"

namespace Engine {
    class CameraNode : public Node {
    public:
        CameraNode();
        void Init() override;
        void Process(float deltaTime) override;
        void Draw(Renderer &) override;

        void lookAt(float eyex, float eyey, float eyez, float centerx, float centery, float centerz);

    };



    REGISTER_CLASS(CameraNode);
}



#endif //GP_FRAMEWORK_CAMERANODE_H