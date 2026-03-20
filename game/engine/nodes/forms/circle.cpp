#include "circle.h"

#include <cassert>
#include <numbers>

#include "../../logmanager/logmanager.h"


namespace Engine {
    Circle::Circle(const Transform& t): Form(t) {}
    Circle::~Circle() = default;

    float Circle::GetArea() {
        return static_cast<float>(m_transform.GetHeight() * m_transform.GetWidth() * std::numbers::pi);
    }

    Vector2d Circle::GetFurthestPointTowards(Vector2d pos) const {
        // distance between 2D Vectors
        const float dist = m_transform.position.Distance(pos);

        // ovals do not work yet
        // only overlap in round circles can be calculated
        assert(m_transform.GetHeight() == m_transform.GetWidth()) ;

        const float factor = 1 / (dist / (m_transform.GetHeight() / 2));
        const Vector2d vectorTof = Vector2d(pos.x - m_transform.position.x, pos.y - m_transform.position.y);
        return ((vectorTof * factor) + m_transform.position);
    }
}
