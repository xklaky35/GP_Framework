#include "Level1.h"

#include "../player.h"
#include "../../../engine/input/input.h"
#include "../../../engine/nodes/nodefactory.h"
#include "../../../engine/scenemanager/scenemanager.h"
#include "../../../engine/time/timer.h"

Level1::Level1() {
    SetupNode("Level1", NT_Custom);
    m_timerDisplay = nullptr;
}

void Level1::Init() {
    Node::Init();
    m_timerDisplay = dynamic_cast<TextControl *>(GetChild("LevelTimer"));
    Timer::GetInstance().Reset();


    m_player = dynamic_cast<Player *>(GetChild("Player"));

    m_pickedObject = dynamic_cast<SpriteNode *>(GetChild("PickedObject"));
    if (m_pickedObject != nullptr) {
        m_pickedObject->m_globalTransform.SetScale(0);
    }
    m_unpickdObject = dynamic_cast<SpriteNode *>(GetChild("UnpickedObject"));
    if (m_unpickdObject != nullptr) {
        m_unpickdObject->m_globalTransform.SetScale(0.3);
    }
    m_dropOffSprite = dynamic_cast<AnimatedSpriteNode *>(GetChild("DropOffPoint"));
    if (m_dropOffSprite != nullptr) {
        m_dropOffSprite->m_globalTransform.SetScale(0);
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
        if (m_dropOffSprite != nullptr)
            m_dropOffSprite->m_globalTransform.SetScale(5);

        if (m_unpickdObject != nullptr)
            m_unpickdObject->m_globalTransform.SetScale(0);

        if (m_pickedObject != nullptr)
            m_pickedObject->m_globalTransform.SetScale(0.3);
    }
    if (!m_player->m_bHasTargetObjectReceived) {
        if (m_dropOffSprite != nullptr)
            m_dropOffSprite->m_globalTransform.SetScale(0);
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

