#include "rectangle.h"

#include "../../logmanager/logmanager.h"

namespace Engine {
    Rectangle::Rectangle(Vector2d pos, const float height, const float width) : Form(pos, height, width) {}
    Rectangle::~Rectangle() = default;

    float Rectangle::GetArea() {
        return 0.f;
    }



    Vector2d Rectangle::GetFurthestPointTowards(const Vector2d pos) const {

        Vector2d tmpPos = m_position;

        /* default:
             - target is next to this on x
             - target is above/below this on y

                    *
             ------
             |    |
             |    |
             |    |
             ------
        */
        float distToTarget = tmpPos.Distance(pos);
        float distToCorner = m_position.Distance(Vector2d(m_position.x - m_width / 2, m_position.y - m_height / 2));
        float factor = 1 / (distToTarget / distToCorner);

        // check if x adjustable
        if (pos.x > m_position.x - (m_width / 2) && pos.x < m_position.x + (m_width / 2)) {



            /* case 1:
                - target is in this on x
                - target is above/below this on y

                    *
                 ------
                 |    |
                 |    |
                 |    |
                 ------
            */


            // set pos.x to match target x
            tmpPos.x = pos.x;
            distToTarget = tmpPos.Distance(pos);
            factor = 1 / (distToTarget / (m_height/2));
        }

        // check if y adjustable
        if (pos.y > m_position.y - (m_height / 2) && pos.y < m_position.y + (m_height / 2)) {

            /* case 2:
                - target is next to this on x
                - target is in this on y


                 ------
                 |    |   *
                 |    |
                 |    |
                 ------
            */
            // set pos.y to match target y
            tmpPos.y = pos.y;
            distToTarget = tmpPos.Distance(pos);
            factor = 1 / (distToTarget / (m_width/2));
        }


        //LogManager::GetInstance().Log(INFO, "dist: %f | adj: (%f, %f) | pos: (%f, %f)", distToTarget, tmpPos.x, tmpPos.y, m_position.x, m_position.y);
        const Vector2d vectorTof = Vector2d(pos.x - tmpPos.x, pos.y - tmpPos.y);

        return ((vectorTof * factor) + tmpPos);
    }

}


