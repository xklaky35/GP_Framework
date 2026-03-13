#ifndef GP_FRAMEWORK_METEORS_H
#define GP_FRAMEWORK_METEORS_H

#include "../../engine/nodes/node.h"
#include "../../engine/nodes/collidernode.h"
#include "../../engine/nodes/spritenode.h"

using namespace Engine;

class Meteors : public Node {
public:
    Meteors();

    void Init() override;
    void Process(float deltaTime) override;

    void Draw(Renderer &) override;
    void DrawDebug() override;
    void OnImpact(const Node*);
    void Spin(float deltaTime) const;
    void SetVelocity(float);
    void SetSpinDirection(signed char);
    void SetDamage(int);
    void SetSpinSpeed(float);
    int GetDamage() const;


private:
    float m_velocity;
    float m_spinSpeed;
    signed char m_spinDirection;
    int m_damage;

    SpriteNode* m_spritenode;
    ColliderNode* m_collider;
};


#endif