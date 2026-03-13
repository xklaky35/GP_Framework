#include "form.h"

namespace Engine {
    bool Form::Overlap(const Form &f) const {
        const Vector2d furthestPointTowardsThis = f.GetFurthestPointTowards(this->m_position);
        const Vector2d furthestPointTowardsF = GetFurthestPointTowards(f.m_position);

        if (m_position.Distance(furthestPointTowardsF) >= m_position.Distance(furthestPointTowardsThis)) {
            return true;
        }
        return false;
    }
}