#include "Level1.h"

#include "../player.h"
#include "../../../engine/input/input.h"
#include "../../../engine/nodes/nodefactory.h"
#include "../../../engine/scenemanager/scenemanager.h"
#include "../../../engine/sound/soundmanager.h"
#include "../../../engine/time/timer.h"

Level1::Level1()
    : m_backgroundSound(nullptr),
      m_pickupSound(nullptr) {

    SetupNode("Level1", NT_Custom);
    m_timerDisplay = nullptr;
}

Level1::~Level1() {
    if (m_backgroundSound != nullptr) {
        m_backgroundSound->stop();
    }
}


void Level1::Init() {
    Node::Init();

    m_backgroundSound = SoundManager::GetInstance().PlaySound("caveBackground.mp3");
    if (m_backgroundSound != nullptr) {
        m_backgroundSound->setMode(FMOD_LOOP_NORMAL);
    }

    m_timerDisplay = dynamic_cast<TextControl *>(GetChild("LevelTimer"));
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


    m_player = dynamic_cast<Player *>(GetChild("Player"));

    m_pickedObject = dynamic_cast<SpriteNode *>(GetChild("PickedObject"));
    if (m_pickedObject != nullptr) {
        m_pickedObject->m_globalTransform.SetScale(0);
    }
    m_unpickdObject = dynamic_cast<SpriteNode *>(GetChild("UnpickedObject"));
    if (m_unpickdObject != nullptr) {
        m_unpickdObject->m_globalTransform.SetScale(0.3);
    }
    m_dropOffLocationSprite = dynamic_cast<AnimatedSpriteNode *>(GetChild("DropOffPoint"));
    if (m_dropOffLocationSprite != nullptr) {
        m_dropOffLocationSprite->m_globalTransform.SetScale(0);
    }
}

void Level1::Process(float deltaTime) {
    Node::Process(deltaTime);

    if (m_player == nullptr) return;


    if (InputManager::GetInstance().GetButtonState(SDLK_ESCAPE) == BS_PRESSED) {
        SceneManager::GetInstance().SetSceneActive("MainMenu");
        return;
    }

    // pickup and drop off location sprite control
    if (m_player->m_bHasTargetObjectReceived) {
        if (m_pickupSound == nullptr) {
            m_pickupSound = SoundManager::GetInstance().PlaySound("pickupItem.mp3");
            m_pickupSound->setVolume(5);
        }
        for (auto enemy : m_enemies) {
            enemy->SetEnraged(true);
            enemy->SetEnragedSpeed();
        }
        if (m_dropOffLocationSprite != nullptr)
            m_dropOffLocationSprite->m_globalTransform.SetScale(5);

        if (m_unpickdObject != nullptr)
            m_unpickdObject->m_globalTransform.SetScale(0);

        if (m_pickedObject != nullptr)
            m_pickedObject->m_globalTransform.SetScale(0.3);
    }
    if (!m_player->m_bHasTargetObjectReceived) {
        m_pickupSound = nullptr;

        for (auto enemy : m_enemies) {
            enemy->SetEnraged(false);
            enemy->SetNormalSpeed();
        }
        if (m_dropOffLocationSprite != nullptr)
            m_dropOffLocationSprite->m_globalTransform.SetScale(0);
        ;
        if (m_unpickdObject != nullptr)
            m_unpickdObject->m_globalTransform.SetScale(0.3);

        if (m_pickedObject != nullptr)
            m_pickedObject->m_globalTransform.SetScale(0);
    }


}

void Level1::SetupParameter(IniParser *parser, const std::string &sectionId) {
    Node::SetupParameter(parser, sectionId);
    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/whoosh/Level1/Level1.ini");
}

