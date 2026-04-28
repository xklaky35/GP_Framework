#ifndef GP_FRAMEWORK_RIGIDBODYNODE_H
#define GP_FRAMEWORK_RIGIDBODYNODE_H
#include "node.h"
#include "box2d/id.h"
#include "box2d/types.h"


namespace Engine {
    class RigidbodyNode : public Engine::Node {
    public:
        RigidbodyNode();
        RigidbodyNode(b2BodyType, float mass, float friction);

        void Init() override;
        void Process(float deltaTime) override;

        Vector2d GetBodyPosition();

        b2Rot GetBodyRotation();

        Vector2d GetBodyVelocity();

        void ApplyForceToCenter(Vector2d vec) const;

        void ApplyImpluseToCenter(Vector2d vec);

        void ResetBody();

        void SetMassData(float mass, Vector2d massCenter, float rotationalInertia);

        b2MassData GetMassData();

        void SetVelocity(Vector2d velocity);

        void SetFriction(float friction);

        float GetFriction();

    private:
        b2BodyId m_bodyId;
        b2BodyType m_bodyType;
        float m_fMass;
        bool m_bIsActive;
        bool m_bIsSleeping;
        bool m_bHasFixedRotation;
        bool m_bIsBullet;

        b2ShapeId m_shapeId;
        float m_fFriction;

        b2Polygon m_bodyPolygon{};
        bool m_bIsStatic;
    };
}

#endif //GP_FRAMEWORK_RIGIDBODYNODE_H