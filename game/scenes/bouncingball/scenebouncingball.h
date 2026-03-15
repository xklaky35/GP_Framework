//
// Created by leon on 15.03.26.
//

#ifndef GP_FRAMEWORK_SCENEBOUNCINGBALL_H
#define GP_FRAMEWORK_SCENEBOUNCINGBALL_H
#include "../../engine/nodes/node.h"


using namespace Engine;
class SceneBouncingBall : public Node {

public:
    SceneBouncingBall();
    ~SceneBouncingBall() override;

    void Init() override;
    void Process(float deltaTime) override;
    void Draw(Renderer &) override;

    void CreateBalls();


private:
    int m_iBallCount;

};


#endif