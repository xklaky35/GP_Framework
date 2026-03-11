#include "../engine/logmanager/logmanager.h"
#include "../engine/nodetree/collidernode.h"
#include "../engine/nodetree/spritenode.h"
#include "../engine/nodetree/forms/circle.h"
#include "../engine/nodetree/forms/form.h"

#include "meteors.h"

Meteors::Meteors() : Node("Meteor"), velocity(0), spinSpeed(0), spinDirection(1), damage(0) {}

void Meteors::Init() {
    Node::Init();

    // setup sprite
    Spritenode* spritenode = new Spritenode("MeteorSprite");
    spritenode->SetSprite("../assets/Sprites/ball.png");
    AddChild(*spritenode);

    // setup collider
    Form *circle = new Circle(m_position, 200);
    ColliderNode *coll = new ColliderNode(circle);
    AddChild(*coll);
}

void Meteors::Process(float deltaTime) {
    Node::Process(deltaTime);

    m_position->y += velocity*deltaTime;
    Spin(deltaTime);
}



void Meteors::Spin(const float deltaTime) const {
    auto sprite = GetChild("MeteorSprite");
    sprite->m_transform->rotation += (spinSpeed * deltaTime) * spinDirection;
    sprite->m_transform->scale = m_transform->scale;
}

void Meteors::Draw(Renderer &renderer) {
    Node::Draw(renderer);
}

void Meteors::OnImpact() const {
    LogManager::GetInstance().Log(INFO, "EINGESCHLAGEN!");
}

void Meteors::SetVelocity(const float v) {
    velocity = v;
}

void Meteors::SetSpinDirection(signed char dir) {
    spinDirection = dir;
}

void Meteors::SetDamage(const int d) {
    damage = d;
}
void Meteors::SetSpinSpeed(const float ss) {
    spinSpeed = ss;
}

int Meteors::GetDamage() const {
    return damage;
}



