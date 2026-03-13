#include "../../engine/nodes/collidernode.h"
#include "spaceship.h"

#include "../../engine/input/input.h"


Spaceship::Spaceship(const char* nodeName) :  Node(nodeName), m_spriteNode(nullptr), m_colliderNode(nullptr) {}
Spaceship::~Spaceship() {
    Node::~Node();

    delete m_spriteNode;
    m_spriteNode = nullptr;
    delete m_colliderNode;
    m_colliderNode = nullptr;
}

void Spaceship::Init() {
    Node::Init();

    // setup sprite
    m_spriteNode = new SpriteNode();
    m_spriteNode->SetSpritePath("../assets/Sprites/spaceship.png");
    m_spriteNode->m_bUseSpriteSize = false;
    m_spriteNode->m_transform->rotation = 180;
    m_spriteNode->m_transform->width = 100;
    m_spriteNode->m_transform->height = 100;
    AddChild(*m_spriteNode);


    // setup collider
    m_colliderNode = new ColliderNode(ft_CIRCLE, "SpaceshipCollider");
    m_colliderNode->RegisterOnEnter<Spaceship>(&Spaceship::OnCollision, *this);
    m_colliderNode->m_transform->height = 100;
    m_colliderNode->m_transform->width = 100;
    AddChild(*m_colliderNode);
}

void Spaceship::OnCollision(const Node* node) {
}

void Spaceship::Process(float deltaTime) {
    Node::Process(deltaTime);
    HandleMovement(deltaTime);
}

void Spaceship::HandleMovement(float deltaTime) {
    velocity = velocity.Zero();
    if (Input::GetCurrentEvents().IsPressed(SDLK_w)) {
        velocity.y = -1;
    }
    if (Input::GetCurrentEvents().IsPressed(SDLK_a)) {
        velocity.x = -1;
    }
    if (Input::GetCurrentEvents().IsPressed(SDLK_s)) {
        velocity.y = 1;
    }
    if (Input::GetCurrentEvents().IsPressed(SDLK_d)) {
        velocity.x = 1;
    }

    *m_position += velocity * speed * deltaTime;
}

void Spaceship::Draw(Renderer &renderer) {
    Node::Draw(renderer);
}
