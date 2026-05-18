
#include "foregroundlayerlevel1.h"

ForegroundLayerLevel1::ForegroundLayerLevel1() {
    SetupNode("ForegroundLayerLevel1", NT_Custom);
}

void ForegroundLayerLevel1::SetupParameter(IniParser *parser, const std::string &sectionId) {
    Node::SetupParameter(parser, sectionId);
    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/whoosh/Level1/ForegroundLayerLevel1.ini");
}
