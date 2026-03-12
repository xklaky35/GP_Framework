#include "../engine/logmanager/logmanager.h"
#include "../engine/nodetree/collidernode.h"
#include "../engine/nodetree/forms/circle.h"
#include "spaceship.h"


Spaceship::Spaceship(const char* nodeName) :  Node(nodeName), velocity(0), speedFactor(0), m_spriteNode(nullptr), m_colliderNode(nullptr) {}
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
    m_spriteNode = new Spritenode();
    m_spriteNode->SetSpritePath("../assets/Sprites/spaceship.png");
    m_spriteNode->m_bUseSpriteSize = false;
    m_spriteNode->m_transform->rotation = 180;
    m_spriteNode->m_transform->width = 100;
    m_spriteNode->m_transform->height = 100;
    AddChild(*m_spriteNode);


    // setup collider
    m_colliderNode = new ColliderNode(ft_RECTANGLE, "SpaceshipCollider");
    m_colliderNode->RegisterOnEnter<Spaceship>(&Spaceship::OnCollision, *this);
    m_colliderNode->m_transform->height = 200;
    m_colliderNode->m_transform->width = 50;
    AddChild(*m_colliderNode);
}

void Spaceship::OnCollision(const Node* node) {
    LogManager::GetInstance().Log(INFO, "EINGESCHLAGEN! in %s", node->name.c_str());
}

void Spaceship::Process(float deltaTime) {
    Node::Process(deltaTime);

    SDL_Event event;
    while( SDL_PollEvent( &event ) != 0){
        /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
        switch( event.type ){
            case SDL_KEYDOWN:
                /* Check the SDLKey values and move change the coords */
                switch( event.key.keysym.sym ){
                    case SDLK_w:
                        m_position->y -= speedFactor * (velocity * deltaTime);
                        break;
                    case SDLK_s:
                        m_position->y += speedFactor * (velocity * deltaTime);
                        break;
                    case SDLK_a:
                        m_position->x -= speedFactor * (velocity * deltaTime);
                        break;
                    case SDLK_d:
                        m_position->x += speedFactor * (velocity * deltaTime);
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void Spaceship::Draw(Renderer &renderer) {
    Node::Draw(renderer);
}
