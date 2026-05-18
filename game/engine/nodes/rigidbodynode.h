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

        [[nodiscard]] b2BodyId GetBodyId() const;
        [[nodiscard]] float GetDensity() const;
        [[nodiscard]] float GetFriction() const;
        [[nodiscard]] b2Rot GetBodyRotation() const;
        [[nodiscard]] b2MassData GetMassData() const;
        [[nodiscard]] Vector2d GetBodyPosition() const;
        [[nodiscard]] Vector2d GetBodyVelocity() const;
        [[nodiscard]] b2MotionLocks GetMotionLocks() const;

        // setter
        void ToggleRotation();
        void SetDensity(float density);
        void SetFriction(float friction);
        void ToggleVerticalMovementLock();
        void ToggleHorizontalMovementLock();
        void SetPositionInMeters(Vector2d pos) const;
        void SetVerticalVelocity(Vector2d velocity) const;
        void SetHorizontalVelocity(Vector2d velocity) const;
        void SetMassData(float mass, Vector2d massCenter, float rotationalInertia);

    private:
        void TrySetupWithCollider();

    private:
        b2BodyId m_bodyId;
        ColliderNode* m_pCollider;
        bool m_bIsActive;
        bool m_bIsSleeping;
        bool m_bHasFixedRotation;
        bool m_bIsBullet;
        bool m_bAngularRotation;
        bool m_bLinearMovementX;
        bool m_bLinearMovementY;
        float m_fFriction;
        float m_fDensity;
        float m_fMass;

    };
}

#endif //GP_FRAMEWORK_RIGIDBODYNODE_H
