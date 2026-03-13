#ifndef GP_FRAMEWORK_CIRCLE_H
#define GP_FRAMEWORK_CIRCLE_H

#include "form.h"

namespace Engine {
    class Circle : public Form {
    public:
        Circle(Vector2d pos, float radius);
        ~Circle() override;
        float GetArea() override;
        Vector2d GetFurthestPointTowards(Vector2d) const override;

    public:
        float m_radius;
    };
}
#endif //GP_FRAMEWORK_CIRCLE_H