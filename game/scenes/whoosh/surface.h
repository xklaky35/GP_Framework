
#ifndef GP_FRAMEWORK_SURFACE_H
#define GP_FRAMEWORK_SURFACE_H
#include "../../engine/nodes/node.h"
#include "../../engine/nodes/nodefactory.h"


using namespace Engine;
class Surface : public Node {
public:
    Surface();
    void Init() override;
    void Process(float deltaTime) override;
    void SystemProcess() override;
    void Draw(Renderer &) override;
    void DrawDebug() override;
    void SetupParameter(IniParser *parser, const std::string &sectionId) override;
};
REGISTER_CLASS(Surface)


#endif //GP_FRAMEWORK_SURFACE_H