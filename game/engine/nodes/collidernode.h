#ifndef GP_FRAMEWORK_COLLIDERNODE_H
#define GP_FRAMEWORK_COLLIDERNODE_H

#include "node.h"
#include "../event.h"
#include "box2d/collision.h"
#include "box2d/id.h"
#include "box2d/types.h"


namespace Engine {

    // BODY TYPE
    static const char* bodyTypes[] = {"Static", "Kinematic", "Dynamic"};
    static constexpr int BODY_TYPE_COUNT = 3;


    // BODY FORM
    enum Formtype {
        FT_RECTANGLE,
        FT_CIRCLE,
        FT_CAPSULE,
        FT_UNDEFINED
    };
    inline const char* FormtypeStrings[] =  {"Rectangle", "Circle", "Capsule"};
    constexpr int FORMTYPE_STRINGS_COUNT = 3;



    // LAYERS
    enum CollisionLayer {
        GROUND = 0x0001,
        WALL = 0x0002,
        ROOF = 0x0003,
        PLAYER = 0x0004,
        ENEMY = 0x0005
    };
    inline const char* CollisionLayerStrings[] =  {"Ground", "Wall", "Roof", "Player", "Enemy"};
    constexpr int LAYER_COUNT = 5;



    // DEFAULT CONFIGURATIONS

    constexpr b2Vec2 DEFAULT_POSITION = b2Vec2(0,0);

    // RECT
    constexpr int DEFAULT_RECT_WIDTH = 10;
    constexpr int DEFAULT_RECT_HEIGHT = 10;

    // CIRCLE
    constexpr int DEFAULT_CIRCLE_RADIUS = 10;


    class ColliderNode : public Node {
    public:
        ColliderNode();

        void Init() override;
        void Process(float deltaTime) override;
        void SetupParameter(IniParser *parser, const std::string &sectionId) override;


        // getter
        [[nodiscard]] bool IsSensor() const;
        [[nodiscard]] b2BodyId GetBodyId() const;
        [[nodiscard]] b2ShapeId GetShapeId() const;
        [[nodiscard]] Vector2d GetRectShape() const;
        [[nodiscard]] b2BodyType GetBodyType() const;
        [[nodiscard]] b2Circle GetCircleShape() const;
        [[nodiscard]] b2Filter GetCurrentFilter() const;
        [[nodiscard]] b2Capsule GetCapsuleShape() const;
        [[nodiscard]] Vector2d GetColliderOffset() const;
        [[nodiscard]] Formtype GetCurrentFormType() const;
        [[nodiscard]] b2ShapeDef GetShapeDefinition() const;
        [[nodiscard]] Vector2d GetBodyPositionInPixel() const;
        [[nodiscard]] Vector2d GetCurrentVelocity() const;
        [[nodiscard]] Vector2d GetBodyPositionInMeter() const;

        // setter
        void ToggleSensor();
        void SetPositionInMeters(b2Vec2) const;
        void SetBodyType(b2BodyType type);
        void SetFilterLayer(int layerNum);
        void SetOffset(Vector2d vector2d);
        void SetFormType(Formtype formtype);
        void SetCircleShape(b2Circle newCircle);
        void SetCapsuleShape(const b2Capsule &newCapsule);
        void SetRectShape(float width, float height);

        Node *GetUserData() const;

        void SetData(Node*) const;


    private:
        void UpdateCurrentForm();
        void UpdateCurrentBodyPosition();
        void CheckForCollision() const;
        void CreateShapeRectangle(float width, float height);
        void CreateShapeCircle(float radius, b2Vec2 center);
        void CreateShapeCapsule(float radius, b2Vec2 center1, b2Vec2 center2);



    public:
        Event<b2ShapeId> OnEntry;
        Event<b2ShapeId> OnExit;

    private:
        b2BodyType m_bodyType;
        Formtype m_currentFormtype;
        Formtype m_selectedFormtype;
        b2BodyId m_colliderBodyId;
        b2Polygon m_colliderPolygon;
        b2ShapeId m_colliderShapeId;
        Vector2d m_colliderOffset;
        Vector2d m_colliderOriginalPosition;
        b2Filter m_layerFilter;
        bool m_bIsSensor;

        // rectangle vars
        float m_fRectWidth = DEFAULT_RECT_WIDTH;
        float m_fRectHeight = DEFAULT_RECT_HEIGHT;

        // circle vars
        float m_fCircleRadius = DEFAULT_CIRCLE_RADIUS;
        b2Vec2 m_circleCenter = DEFAULT_POSITION;

        // capsule vars
        float m_fCapsuleRadius = DEFAULT_CIRCLE_RADIUS;
        b2Vec2 m_capsuleCenter1 = DEFAULT_POSITION;
        b2Vec2 m_capsuleCenter2 = DEFAULT_POSITION;
    };

}
#endif
