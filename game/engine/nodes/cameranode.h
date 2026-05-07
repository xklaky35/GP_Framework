#ifndef GP_FRAMEWORK_CAMERANODE_H
#define GP_FRAMEWORK_CAMERANODE_H
#include "nodefactory.h"
#include "../structs/vector3d.h"

namespace Engine {
    class CameraNode : public Node {
    public:
        CameraNode();
        ~CameraNode() override;
        void Init() override;
        void Process(float deltaTime) override;
        void Draw(Renderer &) override;


    private:
        Vector3d m_pos;
        Vector3d m_direction;
        Vector3d m_rightAxis;
        Vector3d m_upAxis;
        Renderer* m_renderer;

    };



    REGISTER_CLASS(CameraNode);
}



#endif //GP_FRAMEWORK_CAMERANODE_H