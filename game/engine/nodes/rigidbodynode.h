#ifndef GP_FRAMEWORK_RIGIDBODYNODE_H
#define GP_FRAMEWORK_RIGIDBODYNODE_H
#include "collidernode.h"
#include "node.h"
#include "box2d/id.h"
#include "box2d/types.h"


namespace Engine {



    class RigidbodyNode : public Node {

    public:
        RigidbodyNode();
        RigidbodyNode(b2BodyType, float mass, float friction);

        void Init() override;
        void Process(float deltaTime) override;
        void SetupParameter(IniParser *parser, const std::string &sectionId) override;

        void ResetBodyVelocity() const;
        void ApplyForceToCenter(Vector2d vec) const;
        void ApplyImpluseToCenter(Vector2d vec) const;

        // getter

        b2BodyId GetBodyId();
        float GetDensity() const;
        float GetFriction() const;
        b2Rot GetBodyRotation() const;
        b2MassData GetMassData() const;
        Vector2d GetBodyPosition() const;
        Vector2d GetBodyVelocity() const;
        b2MotionLocks GetMotionLocks() const;

        // setter
        void ToggleRotation();
        void SetDensity(float density);
        void SetFriction(float friction);
        void ToggleVerticalMovementLock();
        void ToggleHorizontalMovementLock();
        void SetPositionInMeters(Vector2d pos);
        void SetVerticalVelocity(Vector2d velocity) const;
        void SetHorizontalVelocity(Vector2d velocity) const;
        void SetMassData(float mass, Vector2d massCenter, float rotationalInertia);

    private:
        void TrySetupWithCollider();

    private:
        b2BodyId m_bodyId;
        ColliderNode* m_collider;

        float m_fMass;
        bool m_bIsActive;
        bool m_bIsSleeping;
        bool m_bHasFixedRotation;
        bool m_bIsBullet;
        float m_fFriction;
        float m_fDensity;
        bool m_bAngularRotation;
        bool m_bLinearMovementX;
        bool m_bLinearMovementY;
    };
}

#endif //GP_FRAMEWORK_RIGIDBODYNODE_H
