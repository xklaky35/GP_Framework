#ifndef GP_FRAMEWORK_BOUNCINGBALL_H
#define GP_FRAMEWORK_BOUNCINGBALL_H
#include "../../engine/nodes/collidernode.h"
#include "../../engine/nodes/node.h"
#include "../../engine/nodes/spritenode.h"

using namespace Engine;

class BouncingBall : public Node {

public:

    BouncingBall();
    ~BouncingBall() override;
    void Init() override;
    void Process(float deltaTime) override;

    void RandomizeSize();
    void ComputeBoundary(int, int);

    void RandomizeColor();

private:
    SpriteNode* m_ballSprite;
    Vector2d m_velocity;
    Vector2d m_boundaryHeight;
    Vector2d m_boundaryWidth;
};



#endif //GP_FRAMEWORK_BOUNCINGBALL_H