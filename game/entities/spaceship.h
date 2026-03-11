#ifndef GP_FRAMEWORK_RECTANGLE_H
#define GP_FRAMEWORK_RECTANGLE_H

#include "../engine/nodetree/node.h"
#include "../engine/nodetree/spritenode.h"
#include "../engine/nodetree/collidernode.h"

using namespace Engine;

class Spaceship : public Node {
public:
    Spaceship(const char* = "Spaceship");
    ~Spaceship() override;

    void Init() override;

    void OnCollision(Node *node);

    void Process(float deltaTime) override;
    void Draw(Renderer &) override;


public:
    float velocity;
    int speedFactor;

    // child setup
    Spritenode* m_spriteNode;
    ColliderNode* m_colliderNode;



};


#endif //GP_FRAMEWORK_RECTANGLE_H