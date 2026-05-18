#include "Level1.h"

#include "../player.h"
#include "../../../engine/input/input.h"
#include "../../../engine/nodes/nodefactory.h"
#include "../../../engine/scenemanager/scenemanager.h"
#include "../../../engine/sound/soundmanager.h"
#include "../../../engine/time/timer.h"

Level1::Level1()
    : m_pTimerDisplay(nullptr),
      m_pPlayer(nullptr),
      m_pDropOffLocationSprite(nullptr),
      m_pPickedObject(nullptr),
      m_pUnpickdObject(nullptr),
      m_pBackgroundSound(nullptr),
      m_pPickupSound(nullptr) {

    SetupNode("Level1", NT_Custom);
    m_pTimerDisplay = nullptr;
}

Level1::~Level1() {
    if (m_pBackgroundSound != nullptr) {
        m_pBackgroundSound->stop();
    }
}


void Level1::Init() {
    Node::Init();

    m_pBackgroundSound = SoundManager::GetInstance().PlaySound("caveBackground.mp3");
    if (m_pBackgroundSound != nullptr) {
        m_pBackgroundSound->setMode(FMOD_LOOP_NORMAL);
    }

    m_pTimerDisplay = dynamic_cast<TextControl *>(GetChild("LevelTimer"));
    Timer::GetInstance().Reset();

    if (auto enemy = dynamic_cast<Enemy*>(GetChild("Enemy1"))) {
        m_enemies.push_back(enemy);
    }
    if (auto enemy = dynamic_cast<Enemy*>(GetChild("Enemy2"))) {
        m_enemies.push_back(enemy);
    }
    if (auto enemy = dynamic_cast<Enemy*>(GetChild("Enemy3"))) {
        m_enemies.push_back(enemy);
    }


    m_pPlayer = dynamic_cast<Player *>(GetChild("Player"));

    m_pPickedObject = dynamic_cast<SpriteNode *>(GetChild("PickedObject"));
    if (m_pPickedObject != nullptr) {
        m_pPickedObject->m_globalTransform.SetScale(0);
    }
    m_pUnpickdObject = dynamic_cast<SpriteNode *>(GetChild("UnpickedObject"));
    if (m_pUnpickdObject != nullptr) {
        m_pUnpickdObject->m_globalTransform.SetScale(0.3);
    }
    m_pDropOffLocationSprite = dynamic_cast<AnimatedSpriteNode *>(GetChild("DropOffPoint"));
    if (m_pDropOffLocationSprite != nullptr) {
        m_pDropOffLocationSprite->m_globalTransform.SetScale(0);
    }
}

void Level1::Process(float deltaTime) {
    Node::Process(deltaTime);

    if (m_pPlayer == nullptr) return;


    if (InputManager::GetInstance().GetButtonState(SDLK_ESCAPE) == BS_PRESSED) {
        SceneManager::GetInstance().SetSceneActive("MainMenu");
        return;
    }

    // pickup and drop off location sprite control
    if (m_pPlayer->m_bHasTargetObjectReceived) {
        if (m_pPickupSound == nullptr) {
            m_pPickupSound = SoundManager::GetInstance().PlaySound("pickupItem.mp3");
            m_pPickupSound->setVolume(5);
        }
        for (auto enemy : m_enemies) {
            enemy->SetEnraged(true);
            enemy->SetEnragedSpeed();
        }
        if (m_pDropOffLocationSprite != nullptr)
            m_pDropOffLocationSprite->m_globalTransform.SetScale(5);

        if (m_pUnpickdObject != nullptr)
            m_pUnpickdObject->m_globalTransform.SetScale(0);

        if (m_pPickedObject != nullptr)
            m_pPickedObject->m_globalTransform.SetScale(0.3);
    }
    if (!m_pPlayer->m_bHasTargetObjectReceived) {
        m_pPickupSound = nullptr;

        for (auto enemy : m_enemies) {
            enemy->SetEnraged(false);
            enemy->SetNormalSpeed();
        }
        if (m_pDropOffLocationSprite != nullptr)
            m_pDropOffLocationSprite->m_globalTransform.SetScale(0);
        ;
        if (m_pUnpickdObject != nullptr)
            m_pUnpickdObject->m_globalTransform.SetScale(0.3);

        if (m_pPickedObject != nullptr)
            m_pPickedObject->m_globalTransform.SetScale(0);
    }


}

void Level1::SetupParameter(IniParser *parser, const std::string &sectionId) {
    Node::SetupParameter(parser, sectionId);
    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/whoosh/Level1/Level1.ini");
}

