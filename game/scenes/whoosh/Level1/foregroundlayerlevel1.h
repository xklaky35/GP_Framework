#ifndef GP_FRAMEWORK_FOREGROUNDLAYER_H
#define GP_FRAMEWORK_FOREGROUNDLAYER_H
#include "../../../engine/nodes/node.h"
#include "../../../engine/nodes/nodefactory.h"

using namespace Engine;

class ForegroundLayerLevel1 : public Node {
    public:
    ForegroundLayerLevel1();
    void SetupParameter(IniParser *parser, const std::string &sectionId) override;

};
REGISTER_CLASS(ForegroundLayerLevel1);


#endif //GP_FRAMEWORK_FOREGROUNDLAYER_H
