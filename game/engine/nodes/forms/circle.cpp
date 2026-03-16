#include "circle.h"

#include <cmath>
#include <numbers>

#include "../../logmanager/logmanager.h"

namespace Engine {
    Circle::Circle(Vector2d pos, const float radius): Form(pos, radius*2,radius*2),m_radius(radius) {}
    Circle::~Circle() = default;

    float Circle::GetArea() {
        return static_cast<float>(std::numbers::pi * std::pow(m_radius * m_scale, 2));
    }

    Vector2d Circle::GetFurthestPointTowards(Vector2d pos) const {
        // distance between 2D Vectors
        const float dist = m_position.Distance(pos);

        //LogManager::GetInstance().Log(INFO, "Distance: %f", dist);

        const float factor = 1 / (dist / (m_radius * m_scale));
        const Vector2d vectorTof = Vector2d(pos.x - m_position.x, pos.y - m_position.y);
        return ((vectorTof * factor) + m_position);
    }
}
