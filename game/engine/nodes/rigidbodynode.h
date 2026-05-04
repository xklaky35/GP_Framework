#ifndef GP_FRAMEWORK_RIGIDBODYNODE_H
#define GP_FRAMEWORK_RIGIDBODYNODE_H
#include "node.h"
#include "box2d/id.h"
#include "box2d/types.h"


namespace Engine {


    static const char* bodyTypes[] = {"Static", "Kinematic", "Dynamic"};
    static constexpr int BODY_TYPE_COUNT = 3;

    class RigidbodyNode : public Node {

    public:
        RigidbodyNode();
        RigidbodyNode(b2BodyType, float mass, float friction);

        void Init() override;
        void Process(float deltaTime) override;

        void ApplyForceToCenter(Vector2d vec) const;
        void ApplyImpluseToCenter(Vector2d vec) const;
        void ResetBodyVelocity() const;
        void CreateBoxShape(float halveWidth, float halfHeight);

        // getter
        float GetDensity() const;

        void ToggleRotation() const;

        void ToggleHorizontalMovementLock() const;

        void ToggleVerticalMovementLock() const;

        b2MotionLocks GetMotionLocks() const;

        float GetFriction() const;
        b2Rot GetBodyRotation() const;
        b2MassData GetMassData() const;
        b2BodyType GetBodyType() const;
        Vector2d GetBodyPosition() const;
        Vector2d GetBodyVelocity() const;

        // setter
        void SetDensity(float density) const;
        void SetFriction(float friction) const;
        void SetBodyType(b2BodyType type) const;
        void SetVelocity(Vector2d velocity) const;
        void SetPositionInMeters(Vector2d pos) const;
        void SetupParameter(IniParser *parser, const std::string &sectionId) override;
        void SetMassData(float mass, Vector2d massCenter, float rotationalInertia) const;
        b2BodyId GetBodyId();

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
        float m_fDensity;
        bool m_bAngularRotation;
        bool m_bLinearMovementX;
        bool m_bLinearMovementY;

        b2Polygon m_bodyPolygon{};
        bool m_bIsStatic;
    };
}

#endif //GP_FRAMEWORK_RIGIDBODYNODE_H