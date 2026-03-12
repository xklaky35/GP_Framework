#ifndef GP_FRAMEWORK_SPINNINGRECT_H
#define GP_FRAMEWORK_SPINNINGRECT_H
#include "../engine/nodetree/spritenode.h"

using namespace Engine;

class SpinningRect : public Spritenode {

public:
    SpinningRect(float,float,float, const char*);
    void Process(float deltaTime) override;
    void Spin(float);

public:
    float m_spinSpeed;

};


#endif //GP_FRAMEWORK_SPINNINGRECT_H