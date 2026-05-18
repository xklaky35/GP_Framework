#include "debugformscreator.h"

#include "physicsmanager.h"
#include "box2d/box2d.h"
#include "box2d/types.h"

namespace Engine {
    void DebugFormsCreator::CreateDebugRect(Vector2d pos, float width, float height) {
        b2BodyDef tmpBodyDef = b2DefaultBodyDef();
        tmpBodyDef.position = pos;
        tmpBodyDef.type = b2_staticBody;
        auto tmpBody = b2CreateBody(PhysicsManager::GetInstance().GetWorld(), &tmpBodyDef);
        b2ShapeDef tmpShape = b2DefaultShapeDef();
        tmpShape.isSensor = true;
        b2Polygon tmpPolygon = b2MakeBox(width/2,height/2);
        b2CreatePolygonShape(tmpBody, &tmpShape, &tmpPolygon);
    }
}
