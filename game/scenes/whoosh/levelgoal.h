#ifndef GP_FRAMEWORK_LEVELGOAL_H
#define GP_FRAMEWORK_LEVELGOAL_H
#include "../../engine/nodes/collidernode.h"
#include "../../engine/nodes/node.h"
#include "../../engine/nodes/nodefactory.h"

using namespace Engine;

class LevelGoal : public Node{
public:
    LevelGoal();
    void Init() override;
    void SetupParameter(IniParser *parser, const std::string &sectionId) override;

private:
    ColliderNode* m_collisionArea;

};
REGISTER_CLASS(LevelGoal)


#endif //GP_FRAMEWORK_LEVELGOAL_H