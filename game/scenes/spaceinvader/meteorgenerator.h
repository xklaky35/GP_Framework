#ifndef GP_FRAMEWORK_METHEORGENERATOR_H
#define GP_FRAMEWORK_METHEORGENERATOR_H

#include "../../engine/nodes/node.h"

using namespace Engine;

class MeteorGenerator : public Node {
public:
    MeteorGenerator(int);
    void Process(float deltaTime) override;
    void GenerateMetheor();

private:
    int m_meteorCount;
    int m_maxMeteorCount;
};



#endif //GP_FRAMEWORK_METHEORGENERATOR_H
