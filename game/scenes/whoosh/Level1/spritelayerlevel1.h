#ifndef GP_FRAMEWORK_SPRITELAYER_H
#define GP_FRAMEWORK_SPRITELAYER_H
#include "../../../engine/nodes/node.h"
#include "../../../engine/nodes/nodefactory.h"


using namespace Engine;
class SpriteLayerLevel1 : public Node {
public:
    SpriteLayerLevel1();
    void SetupParameter(IniParser *parser, const std::string &sectionId) override;
};
REGISTER_CLASS(SpriteLayerLevel1)


#endif //GP_FRAMEWORK_SPRITELAYER_H