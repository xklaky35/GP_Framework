//
// Created by leon on 07.05.26.
//

#include "levelgoal.h"


LevelGoal::LevelGoal() {
    SetupNode("LevelGoal", NT_Custom);
}
void LevelGoal::SetupParameter(IniParser *parser, const std::string &sectionId) {
    Node::SetupParameter(parser, sectionId);
    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/whoosh/LevelGoal.ini");
}
