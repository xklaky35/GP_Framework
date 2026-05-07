#ifndef GP_FRAMEWORK_LEVELGOAL_H
#define GP_FRAMEWORK_LEVELGOAL_H
#include "../../engine/nodes/node.h"
#include "../../engine/nodes/nodefactory.h"

using namespace Engine;

class LevelGoal : public Node{
public:
    LevelGoal();
    void SetupParameter(IniParser *parser, const std::string &sectionId) override;
};
REGISTER_CLASS(LevelGoal)


#endif //GP_FRAMEWORK_LEVELGOAL_H