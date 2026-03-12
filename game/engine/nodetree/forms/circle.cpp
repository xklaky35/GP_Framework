#include "circle.h"

#include <cmath>
#include <numbers>

#include "../../logmanager/logmanager.h"

namespace Engine {
    Circle::Circle(Vector2d pos, const float radius): Form(pos, radius,radius),m_radius(radius) {}
    Circle::~Circle() = default;

    float Circle::GetArea() {
        return static_cast<float>(std::numbers::pi * std::pow(m_radius, 2));
    }

    bool Circle::Overlap(const Form &f) const {
        const Vector2d furthestPointTowardsThis = f.GetFurthestPointTowards(this->m_position);
        const Vector2d furthestPointTowardsF = GetFurthestPointTowards(f.m_position);

        if (m_position.Distance(furthestPointTowardsF) >= m_position.Distance(furthestPointTowardsThis)) {
            return true;
        }
        return false;
    }

    Vector2d Circle::GetFurthestPointTowards(Vector2d pos) const {
        // distance between 2D Vectors
        const float dist = m_position.Distance(pos);

        //LogManager::GetInstance().Log(INFO, "Distance: %f", dist);

        const float factor = 1 / (dist / m_radius);
        const Vector2d vectorTof = Vector2d(pos.x - m_position.x, pos.y - m_position.y);
        return ((vectorTof * factor) + m_position);
    }
}
