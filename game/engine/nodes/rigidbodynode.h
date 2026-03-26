#ifndef GP_FRAMEWORK_RIGIDBODYNODE_H
#define GP_FRAMEWORK_RIGIDBODYNODE_H
#include "node.h"
#include "box2d/id.h"
#include "box2d/types.h"


namespace Engine {
    class RigidbodyNode : public Engine::Node {
    public:
        RigidbodyNode(b2BodyType);

        void Init() override;
        void Process(float deltaTime) override;
        void AddForceToCenter(Vector2d vec);

    private:
        b2BodyId m_bodyId;
        b2BodyType m_bodyType;
        b2Polygon m_bodyPolygon;
        bool m_bIsStatic;
    };
}

#endif //GP_FRAMEWORK_RIGIDBODYNODE_H