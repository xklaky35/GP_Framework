#ifndef GP_FRAMEWORK_BULLET_H
#define GP_FRAMEWORK_BULLET_H

#include "../../engine/nodes/spritenode.h"
#include "../../engine/nodes/collidernode.h"

using namespace Engine;

class Bullet : public SpriteNode {
public:

    Bullet(float speed, float ttl, Vector2d direction, Vector2d pos);

    void Init() override;
    void Process(float) override;


private:
    Vector2d m_dir;
    Vector2d m_pos;
    float m_speed;
    float m_ttl;
    float m_deltaSum;

    ColliderNode* m_colliderNode;
};


#endif