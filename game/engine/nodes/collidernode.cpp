#include "collidernode.h"

#include "imgui.h"
#include "nodefactory.h"
#include "../../helper/inlinehelper.h"
#include "../logmanager/logmanager.h"
#include "../physics/physicsmanager.h"
#include "box2d/box2d.h"
#include "box2d/types.h"

namespace Engine {
    ColliderNode::ColliderNode() {
        OnEntry = Event<b2ShapeId>();
        OnExit = Event<b2ShapeId>();
        m_selectedFormtype = FT_RECTANGLE;
        m_currentFormtype = FT_UNDEFINED;
        m_bIsSensor = true;


        SetupNode("ColliderNode", NT_ColliderNode);
        m_nodeInfo.push_back(
            {
                "", [](Node &n) {
                    ImGui::Separator();
                    ImGui::Text("RigidBody Settings");
                }
            });
        m_nodeInfo.push_back(
            {
                "CollisionLayer", [](Node &n) {
                    if (auto *s = dynamic_cast<ColliderNode *>(&n)) {
                        if (ImGui::BeginCombo("Layer", "Select collision layer")) {
                            for (int i = 0; i < LAYER_COUNT; i++) {
                                bool isSelected = s->GetCurrentFilter().categoryBits == static_cast<CollisionLayer>(i+1);
                                if (ImGui::Selectable(CollisionLayerStrings[i], isSelected)) {
                                    s->SetFilterLayer(i+1);
                                    s->SetValue("layerNumber", i+1);
                                }
                            }
                            ImGui::EndCombo();
                        }
                    }
                }
            });
        m_nodeInfo.push_back({
                "IsSensor", [](Node &n) {
                    if (auto *s = dynamic_cast<ColliderNode *>(&n)) {
                        auto isSensor = s->IsSensor();
                        if (ImGui::Checkbox("IsSensor", &isSensor)) {
                            s->SetValue("isSensor", isSensor);
                            s->ToggleSensor();
                        }


                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "Form Type", [](Node &n) {
                    if (auto *s = dynamic_cast<ColliderNode *>(&n)) {
                        if (ImGui::BeginCombo("", "Select form type")) {
                            for (int i = 0; i < 3; i++) {
                                bool is_selected =
                                        s->GetCurrentFormType() == static_cast<Formtype>(i);
                                if (ImGui::Selectable(FormtypeStrings[i], is_selected)) {
                                    s->SetFormType(static_cast<Formtype>(i));
                                    s->SetValue("formType", FormtypeStrings[i]);
                                }
                            }
                            ImGui::EndCombo();
                        }
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "Form offset", [](Node &n) {
                    if (auto *s = dynamic_cast<ColliderNode *>(&n)) {
                        // only make the field editable if condition is met
                        Vector2d tmpOffsetPos = s->GetColliderOffset();
                        if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &tmpOffsetPos.x, 2, 0.5f, nullptr, nullptr)) {

                            s->SetValue("formOffsetX", tmpOffsetPos.x);
                            s->SetValue("formOffsetY", tmpOffsetPos.y);
                            s->SetOffset(tmpOffsetPos);
                        }
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "Form configuration", [](Node &n) {
                    if (auto *s = dynamic_cast<ColliderNode *>(&n)) {
                        // only make the field editable if condition is met
                        Vector2d tmpOffsetPos = Vector2d::Zero();

                        if (s->GetCurrentFormType() == FT_RECTANGLE) {
                            Vector2d size = s->GetRectShape();

                            ImGui::Text("Size W - H");
                            if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &size.x, 2, 0.5f, nullptr, nullptr)) {
                                s->SetValue("rectWidth", size.x);
                                s->SetValue("rectHeight", size.y);
                                s->SetRectShape(size.x, size.y);
                            }
                        }

                        if (s->GetCurrentFormType() == FT_CIRCLE) {
                            b2Circle oldCircle = s->GetCircleShape();

                            ImGui::Text("Radius");
                            if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &oldCircle.radius, 1, 0.5f, nullptr, nullptr)) {
                                s->SetValue("circleRadius", tmpOffsetPos.x);
                                s->SetCircleShape(oldCircle);
                            }
                            ImGui::Text("Center X - Y");
                            if (ImGui::DragScalarN("##Editor1", ImGuiDataType_Float, &oldCircle.center.x, 2, 0.5f, nullptr, nullptr)) {
                                s->SetValue("circlePositionX", oldCircle.center.x);
                                s->SetValue("circlePositionY", oldCircle.center.y);
                                s->SetCircleShape(oldCircle);
                            }
                        }
                        if (s->GetCurrentFormType() == FT_CAPSULE) {
                            b2Capsule oldCapsule = s->GetCapsuleShape();

                            ImGui::Text("Radius");
                            if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &oldCapsule.radius, 1, 0.5f, nullptr, nullptr)) {
                                s->SetValue("capsuleRadius", oldCapsule.radius);
                                s->SetCapsuleShape(oldCapsule);
                            }
                            ImGui::Text("Center1 X - Y");
                            if (ImGui::DragScalarN("##Editor1", ImGuiDataType_Float, &oldCapsule.center1.x, 2, 0.5f, nullptr, nullptr)) {
                                s->SetValue("capsulePosition1X", oldCapsule.center1.x);
                                s->SetValue("capsulePosition1Y", oldCapsule.center1.y);
                                s->SetCapsuleShape(oldCapsule);
                            }
                            ImGui::Text("Center2 X - Y");
                            if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &oldCapsule.center2.x, 2, 0.5f, nullptr, nullptr)) {
                                s->SetValue("capsulePosition2X", oldCapsule.center2.x);
                                s->SetValue("capsulePosition2Y", oldCapsule.center2.y);
                                s->SetCapsuleShape(oldCapsule);
                            }
                        }
                    }
                }
            }
        );
        m_nodeInfo.push_back(
            {
                "Bodytype", [](Node &n) {
                    if (auto *s = dynamic_cast<ColliderNode *>(&n)) {
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
                "Position X - Y", [](Node &n) {
                    if (auto *s = dynamic_cast<ColliderNode *>(&n)) {
                        ImGui::SetNextItemWidth(-FLT_MIN);

                        auto pos = s->GetBodyPositionInPixel();
                        ImGui::BeginDisabled();
                        ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &pos, 2, 0.5f, nullptr);
                        ImGui::EndDisabled();
                    }
                }
            }
        );
        m_nodeInfo.push_back({
                "Velocity X - Y", [](Node &n) {
                    if (auto *s = dynamic_cast<ColliderNode *>(&n)) {
                        ImGui::SetNextItemWidth(-FLT_MIN);

                        auto pos = s->GetCurrentVelocity();
                        ImGui::BeginDisabled();
                        ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &pos, 2, 0.5f, nullptr);
                        ImGui::EndDisabled();
                    }
                }
            }
        );
    }

    void ColliderNode::Init() {
        Node::Init();

        // definition
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = m_bodyType;


        // create body with definition
        m_colliderBodyId = b2CreateBody(PhysicsManager::GetInstance().GetWorld(), &bodyDef);

        // creates shape
        UpdateCurrentForm();

        SetPositionInMeters(PhysicsManager::PixelsToMeterVector(m_parent->GetGlobalPosition()));

        // set initial position
        UpdateCurrentBodyPosition();
    }


    void ColliderNode::Process(float deltaTime) {
        Node::Process(deltaTime);

        if (m_parent == nullptr) {
            LogManager::GetInstance().Log(WARNING, "Collider node needs a parent node: [ %s ]", m_UId.c_str());
            return;
        }

        UpdateCurrentForm();
        UpdateCurrentBodyPosition();
        CheckForCollision();
    }

    // only updates the body in the physical world
    void ColliderNode::UpdateCurrentBodyPosition() {
        b2Vec2 currentOffset = PhysicsManager::PixelsToMeterVector(m_colliderOffset);
        m_colliderOriginalPosition = Vector2d{ b2Body_GetPosition(m_colliderBodyId).x - currentOffset.x,  b2Body_GetPosition(m_colliderBodyId).y - currentOffset.y};

        if ( m_parent->m_typeName == NodeTypeStrings[NT_RigidBodyNode]) {
            b2Vec2 currentPos = GetBodyPositionInMeter();
            b2Body_SetTransform(m_colliderBodyId, currentPos + currentOffset, b2MakeRot(0));
        }
        else {
            Vector2d currentPos = PhysicsManager::PixelsToMeterVector(GetGlobalPosition());
            b2Body_SetTransform(m_colliderBodyId,  currentPos + currentOffset, b2MakeRot(0));
        }

    }

    void ColliderNode::SetupParameter(IniParser *parser, const std::string &sectionId) {
        Node::SetupParameter(parser, sectionId);


        m_selectedFormtype = static_cast<Formtype>(
            GetIndexOf(
                FormtypeStrings, parser->GetValueAsString(sectionId, "formType").c_str(), FORMTYPE_STRINGS_COUNT)
        );
        m_bodyType = static_cast<b2BodyType>(
                GetIndexOf(
                        bodyTypes, parser->GetValueAsString(sectionId, "bodyType").c_str(), BODY_TYPE_COUNT
                    )
            );
        m_currentFormtype = FT_UNDEFINED; // it gets set while initializing
        m_bIsSensor = parser->GetValueAsBoolean(sectionId, "isSensor");
        m_layerFilter.categoryBits = parser->GetValueAsInt(sectionId, "layerNumber");
        m_colliderOffset.x = parser->GetValueAsFloat(sectionId, "formOffsetX");
        m_colliderOffset.y = parser->GetValueAsFloat(sectionId, "formOffsetY");
        m_rectWidth = parser->GetValueAsFloat(sectionId, "rectWidth");
        m_rectHeight = parser->GetValueAsFloat(sectionId, "rectHeight");

        m_circleRadius = parser->GetValueAsFloat(sectionId, "circleRadius");
        m_circleCenter.x = parser->GetValueAsFloat(sectionId, "circlePositionX");
        m_circleCenter.y = parser->GetValueAsFloat(sectionId, "circlePositionY");

        m_capsuleRadius = parser->GetValueAsFloat(sectionId, "circleRadius");
        m_capsuleCenter1.x = parser->GetValueAsFloat(sectionId, "capsulePosition1X");
        m_capsuleCenter1.y = parser->GetValueAsFloat(sectionId, "capsulePosition1Y");
        m_capsuleCenter2.x = parser->GetValueAsFloat(sectionId, "capsulePosition2X");
        m_capsuleCenter2.y = parser->GetValueAsFloat(sectionId, "capsulePosition2Y");

    }

    void ColliderNode::UpdateCurrentForm() {
        if (m_currentFormtype == m_selectedFormtype) return;

        if (m_selectedFormtype == FT_RECTANGLE) {
            CreateShapeRectangle(m_rectWidth, m_rectHeight);
            m_currentFormtype = FT_RECTANGLE;
        }
        if (m_selectedFormtype == FT_CAPSULE) {
            CreateShapeCapsule(m_capsuleRadius,m_capsuleCenter1,m_capsuleCenter2);
            m_currentFormtype = FT_CAPSULE;
        }
        if (m_selectedFormtype == FT_CIRCLE) {
            CreateShapeCircle(m_circleRadius, m_circleCenter);
            m_currentFormtype = FT_CIRCLE;
        }
    }


    void ColliderNode::CreateShapeRectangle(float width, float height) {
        if (b2Shape_IsValid(m_colliderShapeId)) {
            b2DestroyShape(m_colliderShapeId, false);
        }

        m_rectHeight = height > 0 ? height : DEFAULT_RECT_HEIGHT;
        m_rectWidth = width > 0 ? width : DEFAULT_RECT_WIDTH;

        b2ShapeDef shapeDef = GetShapeDefinition();

        m_colliderPolygon = b2MakeBox(m_rectWidth/2, m_rectHeight/2);
        m_colliderShapeId = b2CreatePolygonShape(m_colliderBodyId, &shapeDef, &m_colliderPolygon);
    }

    void ColliderNode::CreateShapeCircle(float radius, b2Vec2 center) {
        if (b2Shape_IsValid(m_colliderShapeId)) {
            b2DestroyShape(m_colliderShapeId, false);
        }

        m_circleCenter = center;
        m_circleRadius = radius > 0 ? radius : DEFAULT_CIRCLE_RADIUS;

        b2ShapeDef shapeDef = GetShapeDefinition();

        b2Circle colliderCircle = b2Circle();
        colliderCircle.center = center;
        colliderCircle.radius = m_circleRadius;
        m_colliderShapeId = b2CreateCircleShape(m_colliderBodyId, &shapeDef, &colliderCircle);
    }

    void ColliderNode::CreateShapeCapsule(float radius, b2Vec2 center1, b2Vec2 center2) {

        if (b2Shape_IsValid(m_colliderShapeId)) {
            b2DestroyShape(m_colliderShapeId, false);
        }

        m_capsuleCenter1 = center1;
        m_capsuleCenter2 = center2;
        m_capsuleRadius = radius > 0 ? radius : DEFAULT_CIRCLE_RADIUS;

        b2ShapeDef shapeDef = GetShapeDefinition();

        b2Capsule colliderCapsule = b2Capsule();
        colliderCapsule.radius = m_capsuleRadius;
        colliderCapsule.center1 = center1;
        colliderCapsule.center2 = center2;
        m_colliderShapeId = b2CreateCapsuleShape(m_colliderBodyId, &shapeDef, &colliderCapsule);
    }


    void ColliderNode::CheckForCollision() const {
        if (m_bIsSensor) {
            b2SensorEvents events = b2World_GetSensorEvents(PhysicsManager::GetInstance().GetWorld());
            for (int i = 0; i < events.beginCount; i++) {
                OnEntry.Emit(&events.beginEvents[i].sensorShapeId);
            }
            for (int i = 0; i < events.endCount; i++) {
                OnExit.Emit(&events.endEvents[i].sensorShapeId);
            }
        }
    }


    // ############### GETTER ###############

    bool ColliderNode::IsSensor() const {
        return m_bIsSensor;
    }


    b2Filter ColliderNode::GetCurrentFilter() const {
        return m_layerFilter;
    }

    Formtype ColliderNode::GetCurrentFormType() const {
        return m_currentFormtype;
    }

    Vector2d ColliderNode::GetRectShape() const {
        return Vector2d{m_rectWidth, m_rectHeight};
    }

    b2Circle ColliderNode::GetCircleShape() const {
        if (m_currentFormtype == FT_CIRCLE) {
            return b2Shape_GetCircle(m_colliderShapeId);
        }
        return b2Circle{};
    }

    b2Capsule ColliderNode::GetCapsuleShape() const {
        if (m_currentFormtype == FT_CAPSULE) {
            return b2Shape_GetCapsule(m_colliderShapeId);
        }
        return b2Capsule{};
    }

    b2ShapeId ColliderNode::GetShapeId() const {
        return m_colliderShapeId;
    }

    b2BodyId ColliderNode::GetBodyId() const {
        return m_colliderBodyId;
    }

    Vector2d ColliderNode::GetColliderOffset() const {
        return m_colliderOffset;
    }

    Vector2d ColliderNode::GetBodyPositionInPixel() const {
        return PhysicsManager::MeterToPixelsVector(m_colliderOriginalPosition);
    }

    Vector2d ColliderNode::GetCurrentVelocity() const {
        auto currentVelocity = b2Body_GetLinearVelocity(m_colliderBodyId);
        return Vector2d{currentVelocity.x, currentVelocity.y};
    }

    Vector2d ColliderNode::GetBodyPositionInMeter() const {
        return m_colliderOriginalPosition;
    }


    b2ShapeDef ColliderNode::GetShapeDefinition() const {

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.isSensor = m_bIsSensor;
        shapeDef.enableSensorEvents = true;
        //shapeDef.filter = m_layerFilter;

        return shapeDef;
    }
    b2BodyType ColliderNode::GetBodyType() const {
        return b2Body_GetType(m_colliderBodyId);
    }


    // ############### SETTER ###############

    void ColliderNode::SetFilterLayer(int layerNum) {
        m_layerFilter.categoryBits = static_cast<CollisionLayer>(layerNum);
        //b2Shape_SetFilter(m_colliderShapeId, m_layerFilter);
    }

    void ColliderNode::SetPositionInMeters(b2Vec2 pos) {
        b2Vec2 currentOffset = PhysicsManager::PixelsToMeterVector(m_colliderOffset);
        b2Body_SetTransform(m_colliderBodyId, pos + currentOffset , b2MakeRot(0.f));
    }

    void ColliderNode::SetFormType(Formtype formType) {
        m_selectedFormtype = formType;
    }

    void ColliderNode::SetOffset(Vector2d vector2d) {
        m_colliderOffset = vector2d;
    }

    void ColliderNode::SetRectShape(float width, float height) {
        if (m_currentFormtype == FT_RECTANGLE) {
            CreateShapeRectangle(width, height);
        }
    }
    void ColliderNode::SetCircleShape(b2Circle newCircle) {
        if (m_currentFormtype == FT_CIRCLE) {
            CreateShapeCircle(newCircle.radius, newCircle.center);
        }
    }
    void ColliderNode::SetCapsuleShape(b2Capsule newCapsule) {
        if (m_currentFormtype == FT_CAPSULE) {
            CreateShapeCapsule(newCapsule.radius, newCapsule.center1, newCapsule.center2);
        }
    }
    void ColliderNode::SetBodyType(b2BodyType type) {
        m_bodyType = type;
        b2Body_SetType(m_colliderBodyId, type);
    }

    void ColliderNode::ToggleSensor() {
        m_bIsSensor = !m_bIsSensor;

        if (m_currentFormtype == FT_CIRCLE) {
            CreateShapeCircle(m_circleRadius, m_circleCenter);
        }
        if (m_currentFormtype == FT_CAPSULE) {
            CreateShapeCapsule(m_capsuleRadius, m_capsuleCenter1, m_capsuleCenter2);
        }
        if (m_currentFormtype == FT_RECTANGLE) {
            CreateShapeRectangle(m_rectWidth, m_rectHeight);
        }
    }

}