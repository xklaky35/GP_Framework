//
// Created by leon on 13.05.26.
//

#ifndef GP_FRAMEWORK_ENEMY_H
#define GP_FRAMEWORK_ENEMY_H
#include "fmod.hpp"
#include "../../engine/nodes/collidernode.h"
#include "../../engine/nodes/node.h"
#include "../../engine/nodes/nodefactory.h"


using namespace Engine;

class Enemy : public Node {
public:
    Enemy();
    ~Enemy() override;
    void Init() override;


    void Process(float deltaTime) override;
    void SetupParameter(IniParser *parser, const std::string &sectionId) override;

    float GetSpeed() const;
    void SetSpeed(float speed);
    void SetEnragedSpeed();
    void SetEnraged(bool enraged);
    void SetNormalSpeed();

private:
    void HandleDetectedEnemy(float deltaTime);
    void HandleSoundEffects();
    void HandleAnimations();
    void ChangeAnimation(AnimatedSpriteNode *animation);
    void OnDetection(const b2ShapeId *target);

private:


    Vector2d m_currentTargetPos;
    Vector2d m_originPos;
    bool m_bHasTargetLocated;
    float m_fSpeed;
    float m_fEnragedSpeed;
    float m_fNormalSpeed;
    Vector2d m_velocity;
    bool m_bIsEnraged;

    // components
    ColliderNode* m_detectionArea;
    AnimatedSpriteNode * m_idleAnimation;
    AnimatedSpriteNode * m_chasingAnimation;
    AnimatedSpriteNode * m_currentAnimation;

    Node* m_detectedObject;


    // audio
    FMOD::Channel* m_idleSound;
    FMOD::Channel* m_chasingSound;
};
REGISTER_CLASS(Enemy)


#endif //GP_FRAMEWORK_ENEMY_H