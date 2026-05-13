#include "enemy.h"

#include "imgui.h"
#include "../../engine/physics/physicsmanager.h"
#include "box2d/box2d.h"


Enemy::Enemy()
    : m_bHasTargetLocated(false),
      m_speed(50),
      m_detectionArea(nullptr),
      m_currentAnimation(nullptr) {

    SetupNode("Enemy", NT_Custom);

    m_nodeInfo.push_back({
            "Enemy speed", [](Node &n) {
                if (auto *s = dynamic_cast<Enemy *>(&n)) {
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    auto speed = s->GetSpeed();
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &speed, 1, 0.5f, nullptr, nullptr)) {
                        s->SetValue("speed", speed);
                        s->SetSpeed(speed);
                    }
                }
            }
        }
    );

}

void Enemy::Init() {
    Node::Init();
    m_originPos = GetGlobalPosition();
    m_globalTransform.SetScale(2);
    m_detectionArea = dynamic_cast<ColliderNode *>(GetChild("DetectionArea"));
    if (m_detectionArea != nullptr) {
        m_detectionArea->OnEntry.Register<Enemy>(&Enemy::OnDetection, *this);
    }
    m_idleAnimation = dynamic_cast<AnimatedSpriteNode *>(GetChild("IdleAnimation"));
    if (m_idleAnimation != nullptr) {
        m_idleAnimation->SetRGBA(0,0,0,0);
    }
    m_chasingAnimation = dynamic_cast<AnimatedSpriteNode *>(GetChild("ChasingAnimation"));
    if (m_chasingAnimation != nullptr) {
        m_chasingAnimation->SetRGBA(0,0,0,0);
    }
}

void Enemy::Process(float deltaTime) {
    Node::Process(deltaTime);

    HandleAnimations();
    HandleDetectedEnemy(deltaTime);
    m_bHasTargetLocated = false;
}

void Enemy::SetupParameter(IniParser *parser, const std::string &sectionId) {
    Node::SetupParameter(parser, sectionId);
    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/whoosh/Enemy.ini");

    m_speed = parser->GetValueAsFloat(sectionId, "speed");
}

float Enemy::GetSpeed() const {
    return  m_speed;
}

void Enemy::SetSpeed(float speed) {
    m_speed = speed;
}

void Enemy::HandleDetectedEnemy(float deltaTime) {

    if (m_bHasTargetLocated) {
        auto currentPos = GetGlobalPosition();
        auto direction = (m_currentTargetPos - currentPos);

        // normalise vector
        direction.x = direction.x / currentPos.Distance(m_currentTargetPos);
        direction.y = direction.y / currentPos.Distance(m_currentTargetPos);

        m_velocity =  (direction * deltaTime * m_speed);

        if (b2IsValidVec2(m_velocity)) {
            SetGlobalPosition(currentPos + m_velocity);
        }
    }
    else {
        auto currentPos = GetGlobalPosition();
        auto direction = m_originPos - currentPos;

        // normalise vector
        direction.x = direction.x / currentPos.Distance(m_originPos);
        direction.y = direction.y / currentPos.Distance(m_originPos);

        m_velocity = (direction * deltaTime * m_speed);

        if (b2IsValidVec2(m_velocity)) {
            SetGlobalPosition(currentPos + m_velocity);
        }
    }
}

void Enemy::HandleAnimations() {
    if (m_chasingAnimation == nullptr) return;
    if (m_idleAnimation == nullptr) return;

    if (m_velocity.x > 0 && !m_chasingAnimation->IsFlipped()) {
        m_chasingAnimation->Flip();
    }
    if (m_velocity.x < 0 && m_chasingAnimation->IsFlipped()) {
        m_chasingAnimation->Flip();
    }

    auto isAtOrigin = m_originPos.Compare(GetGlobalPosition(), 5);
    if (m_velocity.x > 0 && !m_idleAnimation->IsFlipped() && !isAtOrigin) {
        m_idleAnimation->Flip();
    }
    if (m_velocity.x < 0 && m_idleAnimation->IsFlipped() && !isAtOrigin) {
        m_idleAnimation->Flip();
    }

    if (!m_bHasTargetLocated) {
        ChangeAnimation(m_idleAnimation);
        return;
    }

    ChangeAnimation(m_chasingAnimation);
}

void Enemy::ChangeAnimation(AnimatedSpriteNode* animation) {
    if (animation == nullptr) return;

    if (m_currentAnimation == nullptr) {
        m_currentAnimation = animation;
    } else {
        m_currentAnimation->SetRGBA(0, 0, 0, 0);
        m_currentAnimation = animation;
    }

    m_currentAnimation->SetRGBA(1, 1, 1, 1);
    m_currentAnimation->SetAnimating(true);
    m_currentAnimation->SetLooping(true);
}

void Enemy::OnDetection(const b2ShapeId* target) {
    if (target == nullptr) return;
    if (!b2Shape_IsValid(target[0])) return;
    if (b2Shape_IsSensor(target[0])) return;
    auto body = b2Shape_GetBody(target[0]);
    if (b2Body_GetType(body) != b2_dynamicBody) return;

    m_currentTargetPos = PhysicsManager::MeterToPixelsVector(Vector2d{b2Body_GetPosition(body).x, b2Body_GetPosition(body).y});
    m_bHasTargetLocated = true;
}


