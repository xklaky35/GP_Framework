//
// Created by leon on 06.05.26.
//

#include "spritelayerlevel1.h"

SpriteLayerLevel1::SpriteLayerLevel1() {
    SetupNode("SpriteLayerLevel1", NT_Custom);
}
void SpriteLayerLevel1::SetupParameter(IniParser *parser, const std::string &sectionId) {
    Node::SetupParameter(parser, sectionId);
    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/whoosh/Level1/SpriteLayerLevel1.ini");
}
