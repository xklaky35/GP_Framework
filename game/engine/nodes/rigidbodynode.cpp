#include "rigidbodynode.h"
#include <cassert>

#include "imgui.h"
#include "nodefactory.h"
#include "../../helper/inlinehelper.h"
#include "../logmanager/logmanager.h"
#include "../physics/physicsmanager.h"
#include "box2d/box2d.h"
#include "box2d/types.h"

namespace Engine {
    RigidbodyNode::RigidbodyNode(const b2BodyType type, const float mass, const float friction) : m_bodyId(), m_fMass(mass), m_bIsActive(false),
                                                    m_bIsSleeping(false),
                                                    m_bHasFixedRotation(false),
                                                    m_bIsBullet(false), m_fFriction(friction), m_collider(nullptr)
                                                     {
        SetupNode("RigidBody", NT_RigidBodyNode);
        m_nodeInfo.push_back(
            {
                "", [](Node &n) {
                    ImGui::Separator();
                    ImGui::Text("RigidBody Settings");
                }
            });
        m_nodeInfo.push_back({
                "Motion Locks", [](Node &n) {
                    if (auto *s = dynamic_cast<RigidbodyNode *>(&n)) {
                        bool rotation = s->GetMotionLocks().angularZ;
                        bool linearX = s->GetMotionLocks().linearX;
                        bool linearY = s->GetMotionLocks().linearY;

                        ImGui::BeginGroup();
                        if (ImGui::Checkbox("Lock Rotation", &rotation)) {
                            s->ToggleRotation();
                            s->SetValue("angularRotation", s->GetMotionLocks().angularZ);
                        }
                        if (ImGui::Checkbox("Lock LinearX", &linearX)) {
                            s->ToggleHorizontalMovementLock();
                            s->SetValue("linearMovementX", s->GetMotionLocks().linearX);
                        }
                        if (ImGui::Checkbox("Lock LinearY", &linearY)) {
                            s->ToggleVerticalMovementLock();
                            s->SetValue("linearMovementY", s->GetMotionLocks().linearY);
                        }
                        ImGui::EndGroup();
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "Density", [](Node &n) {
                    if (auto *s = dynamic_cast<RigidbodyNode *>(&n)) {
                        float tmpDensity = s->GetDensity();
                        if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &tmpDensity, 1, 0.5f, nullptr, nullptr)) {
                            s->SetDensity(tmpDensity);
                            s->SetValue("density", s->GetDensity());
                        }
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "Mass", [](Node &n) {
                    if (auto *s = dynamic_cast<RigidbodyNode *>(&n)) {
                        float tmpMass = s->GetMassData().mass;
                        if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &tmpMass, 1, 0.5f, nullptr, nullptr)) {
                            s->SetMassData(tmpMass, Vector2d(s->GetMassData().center.x,s->GetMassData().center.y), s->GetMassData().rotationalInertia);
                            s->SetValue("mass", s->GetMassData().mass);
                        }
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "Friction", [](Node &n) {
                    if (auto *s = dynamic_cast<RigidbodyNode *>(&n)) {
                        float tmpFric = s->GetFriction();
                        if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &tmpFric, 1, 0.5f, nullptr, nullptr)) {
                            s->SetFriction(tmpFric);
                            s->SetValue("friction", s->GetFriction());
                        }
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "BodyPosition", [](Node &n) {
                    if (auto *s = dynamic_cast<RigidbodyNode *>(&n)) {
                        Vector2d tmpPos = s->GetBodyPosition();

                        ImGui::BeginDisabled();
                        ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &tmpPos, 2, 0.5f, nullptr, nullptr);
                        ImGui::EndDisabled();
                    }
                }
            }
        );
    }
    RigidbodyNode::RigidbodyNode() : RigidbodyNode(b2_dynamicBody, 1,0) {}

    void RigidbodyNode::Init() {
        Node::Init();
        TrySetupWithCollider();
        SetPositionInMeters(PhysicsManager::PixelsToMeterVector(m_parent->GetGlobalPosition()));
    }

    void RigidbodyNode::Process(float deltaTime) {
        Node::Process(deltaTime);

        TrySetupWithCollider();

        // set parent position
        assert(m_parent);
        auto* parent = static_cast<Node *>(b2Body_GetUserData(m_bodyId));
        if (parent != nullptr) {
            parent->m_globalTransform.position = GetBodyPosition();
        }
    }

    void RigidbodyNode::SetupParameter(IniParser *parser, const std::string &sectionId) {
        Node::SetupParameter(parser, sectionId);

        m_fFriction = parser->GetValueAsFloat(sectionId, "friction");
        m_fMass = parser->GetValueAsFloat(sectionId, "mass");
        m_fDensity = parser->GetValueAsFloat(sectionId, "density");
        m_bAngularRotation = parser->GetValueAsBoolean(sectionId, "angularRotation");
        m_bLinearMovementX = parser->GetValueAsBoolean(sectionId, "linearMovementX");
        m_bLinearMovementY = parser->GetValueAsBoolean(sectionId, "linearMovementY");
    }


    // ############# GETTER ################

    // get position in pixels
    Vector2d RigidbodyNode::GetBodyPosition() const {
        if (m_collider == nullptr) return Vector2d{0,0};
        return m_collider->GetBodyPositionInPixel();
    }

    b2Rot RigidbodyNode::GetBodyRotation() const {
        return b2Body_GetRotation(m_bodyId);
    }

    Vector2d RigidbodyNode::GetBodyVelocity() const {
        return {b2Body_GetLinearVelocity(m_bodyId).x, b2Body_GetLinearVelocity(m_bodyId).y};
    }

    b2MassData RigidbodyNode::GetMassData() const {
        return b2Body_GetMassData(m_bodyId);
    }

    float RigidbodyNode::GetFriction() const {
        if (m_collider == nullptr) return 0.f;
        return b2Shape_GetFriction(m_collider->GetShapeId());
    }

    float RigidbodyNode::GetDensity() const {
        if (m_collider == nullptr) return 0.f;
        return b2Shape_GetDensity(m_collider->GetShapeId());
    }

    b2MotionLocks RigidbodyNode::GetMotionLocks() const {
        return b2Body_GetMotionLocks(m_bodyId);
    }

    b2BodyId RigidbodyNode::GetBodyId() {
        return m_bodyId;
    }


    // ############# SETTER ################

    void RigidbodyNode::SetDensity(float density) {
        if (m_collider == nullptr) return;
        m_fDensity = density;
        b2Shape_SetDensity(m_collider->GetShapeId(), density, true);
    }


    void RigidbodyNode::SetMassData(float mass, Vector2d massCenter, float rotationalInertia) {
        auto newMassData = b2MassData();
        m_fMass = mass;
        newMassData.mass = mass;
        newMassData.center = b2Vec2(massCenter.x, massCenter.y);
        newMassData.rotationalInertia = rotationalInertia;
        b2Body_SetMassData(m_bodyId, newMassData);
    }

    void RigidbodyNode::SetHorizontalVelocity(Vector2d velocity) const {
        b2Body_SetLinearVelocity(m_bodyId, b2Vec2(velocity.x, b2Body_GetLinearVelocity(m_bodyId).y));
    }

    void RigidbodyNode::SetVerticalVelocity(Vector2d velocity) const {
        b2Body_SetLinearVelocity(m_bodyId, b2Vec2(b2Body_GetLinearVelocity(m_bodyId).x, velocity.y));
    }

    void RigidbodyNode::SetFriction(float friction) {
        if (m_collider == nullptr) return;
        m_fFriction = friction;
        b2Shape_SetFriction(m_collider->GetShapeId(), friction);
    }



    void RigidbodyNode::SetPositionInMeters(Vector2d pos) const {
        //SetGlobalPosition(PhysicsManager::MeterToPixelsVector(pos));
        if (m_collider != nullptr) {
            m_collider->SetPositionInMeters(pos);
        }
    }

    void RigidbodyNode::ToggleRotation() {
        b2MotionLocks lock = b2MotionLocks();
        lock.angularZ = !b2Body_GetMotionLocks(m_bodyId).angularZ ;
        lock.linearX = b2Body_GetMotionLocks(m_bodyId).linearX ;
        lock.linearY = b2Body_GetMotionLocks(m_bodyId).linearY ;

        m_bAngularRotation = !m_bAngularRotation;

        b2Body_SetMotionLocks(m_bodyId, lock);
    }

    void RigidbodyNode::ToggleHorizontalMovementLock() {
        b2MotionLocks lock = b2MotionLocks();
        lock.angularZ = b2Body_GetMotionLocks(m_bodyId).angularZ ;
        lock.linearX = !b2Body_GetMotionLocks(m_bodyId).linearX ;
        lock.linearY = b2Body_GetMotionLocks(m_bodyId).linearY ;

        m_bLinearMovementX = !m_bLinearMovementX;

        b2Body_SetMotionLocks(m_bodyId, lock);
    }

    void RigidbodyNode::ToggleVerticalMovementLock() {
        b2MotionLocks lock = b2MotionLocks();
        lock.angularZ = b2Body_GetMotionLocks(m_bodyId).angularZ ;
        lock.linearX = b2Body_GetMotionLocks(m_bodyId).linearX ;
        lock.linearY = !b2Body_GetMotionLocks(m_bodyId).linearY ;

        m_bLinearMovementY = !m_bLinearMovementY;

        b2Body_SetMotionLocks(m_bodyId, lock);
    }


    void RigidbodyNode::ApplyForceToCenter(Vector2d vec) const {
        b2Body_ApplyForceToCenter(m_bodyId, b2Vec2{vec.x, vec.y}, true);
    }


    void RigidbodyNode::ApplyImpluseToCenter(Vector2d vec) const {
        b2Body_ApplyLinearImpulseToCenter(m_bodyId, b2Vec2(vec.x, vec.y), true);
    }

    void RigidbodyNode::ResetBodyVelocity() const {
        b2Body_SetLinearVelocity(m_bodyId, b2Vec2(0,0));
    }

    void RigidbodyNode::TrySetupWithCollider() {

        if (m_collider == nullptr) {
            m_collider = dynamic_cast<ColliderNode *>(GetChild("Collider"));

            if (m_collider != nullptr) {
                // create body with definition
                m_bodyId = m_collider->GetBodyId();
                b2Body_SetUserData(m_bodyId, m_parent);
            }
            else {
                // definition
                b2BodyDef bodyDef = b2DefaultBodyDef();
                m_bodyId = b2CreateBody(PhysicsManager::GetInstance().GetWorld(), &bodyDef);
                LogManager::GetInstance().Log(WARNING, "No collider attached to rigid body");
                return;
            }
        }

        SetFriction(m_fFriction);
        SetDensity(m_fDensity);
        SetMassData(m_fMass, Vector2d(GetMassData().center.x,GetMassData().center.y), GetMassData().rotationalInertia);
        b2Body_SetMotionLocks(m_bodyId, b2MotionLocks(m_bLinearMovementX, m_bLinearMovementY, m_bAngularRotation));
    }
}

