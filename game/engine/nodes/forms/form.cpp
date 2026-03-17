#include "form.h"

namespace Engine {
    bool Form::Overlap(const Form &f) const {
        const Vector2d furthestPointTowardsThis = f.GetFurthestPointTowards(this->m_transform.position);
        const Vector2d furthestPointTowardsF = GetFurthestPointTowards(f.m_transform.position);

        if (m_transform.position.Distance(furthestPointTowardsF) >= m_transform.position.Distance(furthestPointTowardsThis)) {
            return true;
        }
        return false;
    }
}