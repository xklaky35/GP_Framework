
#ifndef GP_FRAMEWORK_LEVEL1_H
#define GP_FRAMEWORK_LEVEL1_H

#include "../../../engine/nodes/node.h"
#include "../../../engine/nodes/nodefactory.h"
#include "../../../engine/nodes/ui/textcontrol.h"


using namespace Engine;
class Level1 : public Node {

public:
    Level1();
    void Init() override;
    void Process(float deltaTime) override;
    void SetupParameter(IniParser *parser, const std::string &sectionId) override;

private:
    TextControl* m_timerDisplay;
};
REGISTER_CLASS(Level1)


#endif //GP_FRAMEWORK_LEVEL1_H