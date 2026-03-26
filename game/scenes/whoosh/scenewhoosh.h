#ifndef GP_FRAMEWORK_SCENEWHOOSH_H
#define GP_FRAMEWORK_SCENEWHOOSH_H
#include "player.h"
#include "../../engine/nodes/node.h"
#include "../../engine/nodes/spritenode.h"


using namespace Engine;

class SceneWhoosh : public Node {
public:

    SceneWhoosh();

    void Init() override;


public:

    Player* m_player;
    RigidbodyNode* m_floor;


};


#endif