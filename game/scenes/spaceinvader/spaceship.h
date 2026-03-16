#ifndef GP_FRAMEWORK_RECTANGLE_H
#define GP_FRAMEWORK_RECTANGLE_H

#include "../../engine/nodes/node.h"
#include "../../engine/nodes/spritenode.h"
#include "../../engine/nodes/collidernode.h"

using namespace Engine;

class Spaceship : public Node {
public:
    Spaceship(float, const char* = "Spaceship");
    ~Spaceship() override;

    void Init() override;

    void OnCollision(const Node *node);
    void HandleMovement(float);
    void Process(float deltaTime) override;
    void Draw(Renderer &) override;


public:
    Vector2d velocity;
    float speed;

    // child setup
    SpriteNode* m_spriteNode;
    ColliderNode* m_colliderNode;



};


#endif //GP_FRAMEWORK_RECTANGLE_H