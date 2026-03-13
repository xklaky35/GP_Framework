#ifndef GP_FRAMEWORK_METHEORGENERATOR_H
#define GP_FRAMEWORK_METHEORGENERATOR_H

#include "../../engine/nodes/node.h"

using namespace Engine;

class MetheorGenerator : public Node {
public:
    MetheorGenerator();
    ~MetheorGenerator() override;
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
