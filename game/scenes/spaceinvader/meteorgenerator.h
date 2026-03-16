#ifndef GP_FRAMEWORK_METHEORGENERATOR_H
#define GP_FRAMEWORK_METHEORGENERATOR_H

#include "../../engine/nodes/node.h"

using namespace Engine;

class MeteorGenerator : public Node {
public:
    MeteorGenerator(int);
    ~MeteorGenerator() override;
    void Init() override;
    void Process(float deltaTime) override;
    void Draw(Renderer &) override;
    void DrawDebug() override;
    void GenerateMetheor();
    int GetRandomNumber(int min, int max) const;

private:
    int m_meteorCount;
    int m_maxMeteorCount;
};



#endif //GP_FRAMEWORK_METHEORGENERATOR_H
