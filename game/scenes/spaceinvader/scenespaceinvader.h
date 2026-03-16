#ifndef GP_FRAMEWORK_SCENESPANEINVADER_H
#define GP_FRAMEWORK_SCENESPANEINVADER_H

#include "meteorgenerator.h"
#include "spaceship.h"
#include "../../engine/nodes/node.h"

using namespace Engine;
class SceneSpaceinvader : public Node {

public:

    SceneSpaceinvader();
    ~SceneSpaceinvader() override;

    void Init() override;

private:
    MeteorGenerator* m_metheorGenerator;
    Spaceship* m_spaceship;

};


#endif //GP_FRAMEWORK_SCENESPANEINVADER_H