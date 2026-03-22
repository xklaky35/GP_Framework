#ifndef GP_FRAMEWORK_BULLETSPAWNER_H
#define GP_FRAMEWORK_BULLETSPAWNER_H

#include "../../engine/nodes/node.h"
#include "../../engine/nodes/spritenode.h"

using namespace Engine;

class BulletSpawner : public Node {
public:

    BulletSpawner();

    void Init() override;
    void ShootInDirection(Vector2d);


private:
    float m_fBulletSpeed;

};


#endif //GP_FRAMEWORK_BULLETSPAWNER_H