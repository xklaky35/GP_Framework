//
// Created by leon on 06.05.26.
//

#include "ColliderLayerLevel1.h"

ColliderLayerLevel1::ColliderLayerLevel1() {
    SetupNode("ColliderLayerLevel1", NT_Custom);
}
void ColliderLayerLevel1::SetupParameter(IniParser *parser, const std::string &sectionId) {
    Node::SetupParameter(parser, sectionId);
    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/whoosh/Level1/ColliderLayerLevel1.ini");
}
