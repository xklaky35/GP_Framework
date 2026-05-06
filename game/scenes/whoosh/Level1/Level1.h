
#ifndef GP_FRAMEWORK_LEVEL1_H
#define GP_FRAMEWORK_LEVEL1_H

#include "../../../engine/nodes/node.h"
#include "../../../engine/nodes/nodefactory.h"


using namespace Engine;
class Level1 : public Node {

public:
    Level1();
    void SetupParameter(IniParser *parser, const std::string &sectionId) override;

};
REGISTER_CLASS(Level1)


#endif //GP_FRAMEWORK_LEVEL1_H