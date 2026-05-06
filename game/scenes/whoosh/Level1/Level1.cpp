//
// Created by leon on 06.05.26.
//

#include "Level1.h"

#include "../../../engine/nodes/nodefactory.h"

Level1::Level1() {
    SetupNode("Level1", NT_Custom);
}

void Level1::SetupParameter(IniParser *parser, const std::string &sectionId) {
    Node::SetupParameter(parser, sectionId);
    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/whoosh/Level1/Level1.ini");
}
