#ifndef GP_FRAMEWORK_METEORS_H
#define GP_FRAMEWORK_METEORS_H

#include "../engine/nodetree/node.h"

using namespace Engine;

class Meteors : public Node {
public:
    Meteors();

    void Init() override;
    void Process(float deltaTime) override;

    void Draw(Renderer &) override;
    void OnImpact() const;
    void Spin(float deltaTime) const;
    void SetVelocity(float);
    void SetSpinDirection(signed char);
    void SetDamage(int);
    void SetSpinSpeed(float);
    int GetDamage() const;


private:
    float velocity;
    float spinSpeed;
    signed char spinDirection;
    int damage;

};


#endif