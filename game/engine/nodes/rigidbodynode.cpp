#include "rigidbodynode.h"
#include <cassert>

#include "imgui.h"
#include "nodefactory.h"
#include "../../helper/inlinehelper.h"
#include "../physics/physicsmanager.h"
#include "box2d/box2d.h"
#include "box2d/types.h"

namespace Engine {
    RigidbodyNode::RigidbodyNode(const b2BodyType type, const float mass, const float friction) : m_bodyId(), m_bodyType(type), m_fMass(mass), m_bIsActive(false),
                                                    m_bIsSleeping(false),
                                                    m_bHasFixedRotation(false),
                                                    m_bIsBullet(false), m_shapeId(), m_fFriction(friction),
                                                    m_bIsStatic(true) {
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
        m_nodeInfo.push_back(
            {
                "Bodytype", [](Node &n) {
                    if (auto *s = dynamic_cast<RigidbodyNode *>(&n)) {
                        if (ImGui::BeginCombo("", "Select bodytype")) {
                            for (int i = 0; i < 3; i++) {
                                bool is_selected = s->GetBodyType() == static_cast<b2BodyType>(i);
                                if (ImGui::Selectable(bodyTypes[i], is_selected)) {
                                    s->SetBodyType(static_cast<b2BodyType>(i));
                                    s->SetValue("bodyType",bodyTypes[s->GetBodyType()]);
                                }
                            }
                            ImGui::EndCombo();
                        }
                    }
                }
            });
        m_nodeInfo.push_back({
                "BodyPosition", [](Node &n) {
                    if (auto *s = dynamic_cast<RigidbodyNode *>(&n)) {
                        Vector2d tmpPos = s->GetBodyPosition();
                        int v_min = 0; int v_max = 99999;

                        ImGui::BeginDisabled();
                        ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &tmpPos, 2, 0.5f, &v_min, &v_max);
                        ImGui::EndDisabled();
                    }
                }
            }
        );
    }
    RigidbodyNode::RigidbodyNode() : RigidbodyNode(b2_dynamicBody, 1,0) {}

    void RigidbodyNode::Init() {
        Node::Init();

        // definition
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.position = (b2Vec2){PhysicsManager::PixelsToMeter(m_parent->m_globalTransform.position.x), PhysicsManager::PixelsToMeter(m_parent->m_globalTransform.position.y)};
        bodyDef.userData = m_parent;

        // create body with definition
        m_bodyId = b2CreateBody(PhysicsManager::GetInstance().GetWorld(), &bodyDef);
        SetMassData(m_fMass, Vector2d(0,0), 0);


        // create default shape
        float width = PhysicsManager::PixelsToMeter(GetBaseSize().x > 0 ? GetBaseSize().x : 1);
        float height = PhysicsManager::PixelsToMeter(GetBaseSize().y > 0 ? GetBaseSize().y : 1);
        CreateBoxShape(width, height);

        SetFriction(m_fFriction);
        SetMassData(m_fMass, Vector2d(GetMassData().center.x,GetMassData().center.y), GetMassData().rotationalInertia);
        SetDensity(m_fDensity);
        b2Body_SetMotionLocks(m_bodyId, b2MotionLocks(m_bLinearMovementX, m_bLinearMovementY, m_bAngularRotation));
        SetPositionInMeters(PhysicsManager::PixelsToMeterVector(m_parent->GetGlobalPosition()));
        SetBodyType(m_bodyType);
    }

    void RigidbodyNode::Process(float deltaTime) {
        Node::Process(deltaTime);
        m_globalTransform.position = GetBodyPosition();

        assert(m_parent);
        auto* parent = static_cast<Node *>(b2Body_GetUserData(m_bodyId));
        if (parent != nullptr) {

            parent->m_globalTransform.position = GetBodyPosition();
        }
    }

    void RigidbodyNode::SetupParameter(IniParser *parser, const std::string &sectionId) {
        Node::SetupParameter(parser, sectionId);
        m_bodyType = static_cast<b2BodyType>(
                GetIndexOf(
                        bodyTypes, parser->GetValueAsString(sectionId, "bodyType").c_str(), BODY_TYPE_COUNT
                    )
            );
        m_fFriction = parser->GetValueAsFloat(sectionId, "friction");
        m_fMass = parser->GetValueAsFloat(sectionId, "mass");
        m_fDensity = parser->GetValueAsFloat(sectionId, "density");
        m_bAngularRotation = parser->GetValueAsBoolean(sectionId, "angularRotation");
        m_bLinearMovementX = parser->GetValueAsBoolean(sectionId, "linearMovementX");
        m_bLinearMovementY = parser->GetValueAsBoolean(sectionId, "linearMovementY");
    }



    // get position in pixels
    Vector2d RigidbodyNode::GetBodyPosition() const {
        return PhysicsManager::MeterToPixelsVector({b2Body_GetPosition(m_bodyId).x, b2Body_GetPosition(m_bodyId).y});
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
        return b2Shape_GetFriction(m_shapeId);
    }
    b2BodyType RigidbodyNode::GetBodyType() const {
        return b2Body_GetType(m_bodyId);
    }

    float RigidbodyNode::GetDensity() const {
        return b2Shape_GetDensity(m_shapeId);
    }

    b2MotionLocks RigidbodyNode::GetMotionLocks() const {
        return b2Body_GetMotionLocks(m_bodyId);
    }

    void RigidbodyNode::SetDensity(float density) const {
        b2Shape_SetDensity(m_shapeId, density, true);
    }


    void RigidbodyNode::SetMassData(float mass, Vector2d massCenter, float rotationalInertia) const {
        auto newMassData = b2MassData();
        newMassData.mass = mass;
        newMassData.center = b2Vec2(massCenter.x, massCenter.y);
        newMassData.rotationalInertia = rotationalInertia;
        b2Body_SetMassData(m_bodyId, newMassData);
    }
    void RigidbodyNode::SetVelocity(Vector2d velocity) const {
        b2Body_SetLinearVelocity(m_bodyId, b2Vec2(velocity.x, b2Body_GetLinearVelocity(m_bodyId).y));
    }

    void RigidbodyNode::SetFriction(float friction) const {
        b2Shape_SetFriction(m_shapeId, friction);
    }


    void RigidbodyNode::SetBodyType(b2BodyType type) const {
        b2Body_SetType(m_bodyId, type);
    }


    void RigidbodyNode::SetPositionInMeters(Vector2d pos) const {
        b2Body_SetTransform(m_bodyId, b2Vec2(pos.x, pos.y), b2MakeRot(0.f));
    }


    void RigidbodyNode::CreateBoxShape(float width, float height) {
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        // convert pixel dimensions to meters, fix the swapped args
        m_bodyPolygon = b2MakeOffsetBox(width/2, height/2, b2Vec2(width/2,height/2), b2MakeRot(0));
        m_shapeId = b2CreatePolygonShape(m_bodyId, &shapeDef, &m_bodyPolygon);
    }



    void RigidbodyNode::ToggleRotation() const {
        b2MotionLocks lock = b2MotionLocks();
        lock.angularZ = !b2Body_GetMotionLocks(m_bodyId).angularZ ;
        lock.linearX = b2Body_GetMotionLocks(m_bodyId).linearX ;
        lock.linearY = b2Body_GetMotionLocks(m_bodyId).linearY ;

        b2Body_SetMotionLocks(m_bodyId, lock);
    }

    void RigidbodyNode::ToggleHorizontalMovementLock() const {
        b2MotionLocks lock = b2MotionLocks();
        lock.angularZ = b2Body_GetMotionLocks(m_bodyId).angularZ ;
        lock.linearX = !b2Body_GetMotionLocks(m_bodyId).linearX ;
        lock.linearY = b2Body_GetMotionLocks(m_bodyId).linearY ;

        b2Body_SetMotionLocks(m_bodyId, lock);
    }

    void RigidbodyNode::ToggleVerticalMovementLock() const {
        b2MotionLocks lock = b2MotionLocks();
        lock.angularZ = b2Body_GetMotionLocks(m_bodyId).angularZ ;
        lock.linearX = b2Body_GetMotionLocks(m_bodyId).linearX ;
        lock.linearY = !b2Body_GetMotionLocks(m_bodyId).linearY ;

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
}

