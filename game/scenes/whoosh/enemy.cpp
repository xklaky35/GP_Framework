#include "enemy.h"

#include "imgui.h"
#include "../../engine/physics/physicsmanager.h"
#include "../../engine/sound/soundmanager.h"
#include "box2d/box2d.h"


Enemy::Enemy()
    : m_bHasTargetLocated(false),
      m_fSpeed(50),
      m_fEnragedSpeed(0),
      m_fNormalSpeed(0),
      m_bIsEnraged(false),
      m_pDetectionArea(nullptr),
      m_pIdleAnimation(nullptr),
      m_pChasingAnimation(nullptr),
      m_pCurrentAnimation(nullptr),
      m_detectedObject(nullptr),
      m_idleSound(nullptr),
      m_chasingSound(nullptr) {

    SetupNode("Enemy", NT_Custom);

    m_nodeInfo.push_back({
            "Enemy enraged speed", [](Node &n) {
                if (auto *s = dynamic_cast<Enemy *>(&n)) {
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_fEnragedSpeed, 1, 0.5f, nullptr, nullptr)) {
                        s->SetValue("enragedSpeed", s->m_fEnragedSpeed);
                    }
                }
            }
        }
    );
    m_nodeInfo.push_back({
            "Enemy normal speed", [](Node &n) {
                if (auto *s = dynamic_cast<Enemy *>(&n)) {
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (ImGui::DragScalarN("##Editor", ImGuiDataType_Float, &s->m_fNormalSpeed, 1, 0.5f, nullptr, nullptr)) {
                        s->SetValue("normalSpeed", s->m_fNormalSpeed);
                    }
                }
            }
        }
    );
    m_nodeInfo.push_back({
            "Enemy current speed", [](Node &n) {
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
Enemy::~Enemy() {
    if (m_idleSound != nullptr) {
        m_idleSound->stop();
    }
    if (m_chasingSound != nullptr) {
        m_chasingSound->stop();
    }
}

void Enemy::Init() {
    Node::Init();
    m_originPos = GetGlobalPosition();
    m_globalTransform.SetScale(2);
    m_pDetectionArea = dynamic_cast<ColliderNode *>(GetChild("DetectionArea"));
    if (m_pDetectionArea != nullptr) {
        m_pDetectionArea->OnEntry.Register<Enemy>(&Enemy::OnDetection, *this);
    }
    m_pIdleAnimation = dynamic_cast<AnimatedSpriteNode *>(GetChild("IdleAnimation"));
    if (m_pIdleAnimation != nullptr) {
        m_pIdleAnimation->SetRGBA(0,0,0,0);
    }
    m_pChasingAnimation = dynamic_cast<AnimatedSpriteNode *>(GetChild("ChasingAnimation"));
    if (m_pChasingAnimation != nullptr) {
        m_pChasingAnimation->SetRGBA(0,0,0,0);
    }
}

void Enemy::Process(float deltaTime) {
    Node::Process(deltaTime);

    HandleAnimations();
    HandleDetectedEnemy(deltaTime);
    HandleSoundEffects();

    if (!m_bIsEnraged) {
        m_bHasTargetLocated = false;
    }
}

void Enemy::SetupParameter(IniParser *parser, const std::string &sectionId) {
    Node::SetupParameter(parser, sectionId);
    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/whoosh/Enemy.ini");

    m_fSpeed = parser->GetValueAsFloat(sectionId, "speed");
    m_fNormalSpeed = parser->GetValueAsFloat(sectionId, "normalSpeed");
    m_fEnragedSpeed = parser->GetValueAsFloat(sectionId, "enragedSpeed");
}

float Enemy::GetSpeed() const {
    return  m_fSpeed;
}

void Enemy::SetSpeed(float speed) {
    m_fSpeed = speed;
}

void Enemy::SetEnragedSpeed() {
    m_fSpeed = m_fEnragedSpeed;
}
void Enemy::SetNormalSpeed() {
    m_fSpeed = m_fNormalSpeed;
}

void Enemy::HandleDetectedEnemy(float deltaTime) {
    if (m_detectedObject != nullptr) {
        m_currentTargetPos = m_detectedObject->GetGlobalPosition();
    }

    if (m_bHasTargetLocated) {
        auto currentPos = GetGlobalPosition();
        auto direction = (m_currentTargetPos - currentPos);

        // normalise vector
        direction.x = direction.x / currentPos.Distance(m_currentTargetPos);
        direction.y = direction.y / currentPos.Distance(m_currentTargetPos);

        m_velocity =  (direction * deltaTime * m_fSpeed);

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

        m_velocity = (direction * deltaTime * m_fSpeed);

        if (b2IsValidVec2(m_velocity)) {
            SetGlobalPosition(currentPos + m_velocity);
        }
    }
}

void Enemy::HandleSoundEffects() {
    if (m_bHasTargetLocated) {
        if (m_chasingSound == nullptr) {
            m_chasingSound = SoundManager::GetInstance().PlaySound("grumpyManAggressive.mp3");
            m_chasingSound->setMode(FMOD_LOOP_NORMAL);
        } else {
            if (m_idleSound != nullptr) m_idleSound->stop();
            bool isPlaying;
            m_chasingSound->isPlaying(&isPlaying);

            if (isPlaying == false) {
                m_chasingSound = nullptr;
            }
        }

    } else {
        if (m_idleSound == nullptr) {
            m_idleSound = SoundManager::GetInstance().PlaySound("grumpyManIdle2.mp3");
            m_idleSound->setMode(FMOD_LOOP_NORMAL);
        } else {
            if (m_chasingSound != nullptr) m_chasingSound->stop();
            bool isPlaying;
            m_idleSound->isPlaying(&isPlaying);

            if (isPlaying == false) {
                m_idleSound = nullptr;
            }
        }
    }

}

void Enemy::HandleAnimations() {
    if (m_pChasingAnimation == nullptr) return;
    if (m_pIdleAnimation == nullptr) return;

    if (m_velocity.x > 0 && !m_pChasingAnimation->IsFlipped()) {
        m_pChasingAnimation->Flip();
    }
    if (m_velocity.x < 0 && m_pChasingAnimation->IsFlipped()) {
        m_pChasingAnimation->Flip();
    }

    auto isAtOrigin = m_originPos.Compare(GetGlobalPosition(), 5);
    if (m_velocity.x > 0 && !m_pIdleAnimation->IsFlipped() && !isAtOrigin) {
        m_pIdleAnimation->Flip();
    }
    if (m_velocity.x < 0 && m_pIdleAnimation->IsFlipped() && !isAtOrigin) {
        m_pIdleAnimation->Flip();
    }

    if (!m_bHasTargetLocated) {
        ChangeAnimation(m_pIdleAnimation);
        return;
    }

    ChangeAnimation(m_pChasingAnimation);
}

void Enemy::ChangeAnimation(AnimatedSpriteNode* animation) {
    if (animation == nullptr) return;

    if (m_pCurrentAnimation == nullptr) {
        m_pCurrentAnimation = animation;
    } else {
        m_pCurrentAnimation->SetRGBA(0, 0, 0, 0);
        m_pCurrentAnimation = animation;
    }

    m_pCurrentAnimation->SetRGBA(1, !m_bIsEnraged, !m_bIsEnraged, 1);
    m_pCurrentAnimation->SetAnimating(true);
    m_pCurrentAnimation->SetLooping(true);
}

void Enemy::OnDetection(const b2ShapeId* target) {
    if (target == nullptr) return;
    if (!b2Shape_IsValid(target[0])) return;
    if (b2Shape_IsSensor(target[0])) return;
    auto body = b2Shape_GetBody(target[0]);
    if (b2Body_GetType(body) != b2_dynamicBody) return;

    if (auto obj = static_cast<Node*>(b2Body_GetUserData(body))) {
        m_detectedObject = obj;
    }

    m_bHasTargetLocated = true;
}

void Enemy::SetEnraged(bool enraged) {
    m_bIsEnraged = enraged;
}

