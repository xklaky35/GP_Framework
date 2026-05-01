#include "splashscreen.h"

#include <cassert>

#include "../../config/config.h"
#include "../../engine/nodes/nodefactory.h"
#include "../../engine/scenemanager/scenemanager.h"

using namespace Engine;

Splashscreen::Splashscreen() : m_fFadeSpeed(0.4), m_fCurrentAlphaLevel(0),
                               m_iCurrentScreen(0),
                               m_background(nullptr),
                               m_autLogo(nullptr),
                               m_fmodLogo(nullptr) {
    m_windowHeight = static_cast<float>(Config::GetInstance().windowsHeight);
    m_windowWidth = static_cast<float>(Config::GetInstance().windowsWidth);
}

Splashscreen::~Splashscreen() = default;

void Splashscreen::Init()  {
    Control::Init();

    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/slashscreen/splashscreen.ini");

    m_background = dynamic_cast<SpriteNode *>(GetChild("Background"));
    m_autLogo = dynamic_cast<SpriteNode *>(GetChild("AUTLogo"));
    m_fmodLogo = dynamic_cast<SpriteNode *>(GetChild("FMODLogo"));

    m_screens.push_back(m_autLogo);
    m_screens.push_back(m_fmodLogo);
}

void Splashscreen::Process(float deltaTime) {
    Control::Process(deltaTime);

    if (m_autLogo == nullptr || m_fmodLogo == nullptr || m_background == nullptr)
        return;

    if (m_iCurrentScreen >= m_screens.size()) {
        SceneManager::GetInstance().SetSceneActive("MainMenu");
        return;
    }

    m_screens[m_iCurrentScreen]->SetRGBA(1,1,1, m_fCurrentAlphaLevel +=  m_fFadeSpeed * deltaTime);

    if (m_fCurrentAlphaLevel >= 1) {
        m_fFadeSpeed *= -1;
    }
    if (m_fCurrentAlphaLevel <= 0) {
        m_fCurrentAlphaLevel = 0;
        m_fFadeSpeed *= -1;
        m_screens[m_iCurrentScreen++]->SetRGBA(1,1,1,0);
    }

    m_autLogo->m_globalTransform.position.x = m_windowWidth / 2 - m_autLogo->m_globalTransform.GetWidth() / 2;
    m_autLogo->m_globalTransform.position.y = m_windowHeight / 2 - m_autLogo->m_globalTransform.GetHeight() / 2;
    m_fmodLogo->m_globalTransform.position.x = m_windowWidth / 2 - m_fmodLogo->m_globalTransform.GetWidth() / 2;
    m_fmodLogo->m_globalTransform.position.y = m_windowHeight / 2 - m_fmodLogo->m_globalTransform.GetHeight() / 2;
}
