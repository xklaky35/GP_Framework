#ifndef GP_FRAMEWORK_SCENESPANEINVADER_H
#define GP_FRAMEWORK_SCENESPANEINVADER_H

#include "meteorgenerator.h"
#include "spaceship.h"
#include "../../../lib/BOX2D/include/box2d/id.h"
#include "../../engine/nodes/node.h"

using namespace Engine;
class SceneSpaceinvader : public Node {

public:

    SceneSpaceinvader();

    void Init() override;

private:
    MeteorGenerator* m_metheorGenerator;
    Spaceship* m_spaceship;
    b2WorldId m_worldId;
    b2BodyId m_bodyId;
};


#endif //GP_FRAMEWORK_SCENESPANEINVADER_H