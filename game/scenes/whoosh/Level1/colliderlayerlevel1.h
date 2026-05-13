//
// Created by leon on 06.05.26.
//

#ifndef GP_FRAMEWORK_COLLIDERLAYER_H
#define GP_FRAMEWORK_COLLIDERLAYER_H
#include "../../../engine/nodes/nodefactory.h"


using namespace Engine;
class ColliderLayerLevel1 : public Node{

public:
    ColliderLayerLevel1();
    void SetupParameter(IniParser *parser, const std::string &sectionId) override;

};
REGISTER_CLASS(ColliderLayerLevel1)


#endif //GP_FRAMEWORK_COLLIDERLAYER_H