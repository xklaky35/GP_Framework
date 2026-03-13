//
// Created by leon on 12.03.26.
//

#ifndef GP_FRAMEWORK_BOUNCINGBALL_H
#define GP_FRAMEWORK_BOUNCINGBALL_H
#include "../../engine/nodes/collidernode.h"
#include "../../engine/nodes/node.h"
#include "../../engine/nodes/spritenode.h"

using namespace Engine;

class bouncingball : public Node {

public:

    void Init() override;
    void Process(float deltaTime) override;
    void Draw(Renderer &) override;



private:
    SpriteNode* m_ballSprite;
    ColliderNode* m_ballCollider;

};



#endif //GP_FRAMEWORK_BOUNCINGBALL_H