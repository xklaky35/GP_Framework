#include "splashscreen.h"

#include "../../config/config.h"
#include "../../engine/nodes/nodefactory.h"
#include "../../engine/scenemanager/scenemanager.h"

using namespace Engine;

Splashscreen::Splashscreen()
    : m_fFadeSpeed(0.4),
      m_fCurrentAlphaLevel(0),
      m_iCurrentScreen(0),
      m_pBackground(nullptr),
      m_pAutLogo(nullptr),
      m_pFmodLogo(nullptr) {
    m_fWindowHeight = static_cast<float>(Config::GetInstance().windowsHeight);
    m_fWindowWidth = static_cast<float>(Config::GetInstance().windowsWidth);
}

Splashscreen::~Splashscreen() = default;

void Splashscreen::Init()  {
    Control::Init();

    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/slashscreen/splashscreen.ini");

    m_pBackground = dynamic_cast<SpriteNode *>(GetChild("Background"));
    m_pAutLogo = dynamic_cast<SpriteNode *>(GetChild("AUTLogo"));
    m_pFmodLogo = dynamic_cast<SpriteNode *>(GetChild("FMODLogo"));

    m_screens.push_back(m_pAutLogo);
    m_screens.push_back(m_pFmodLogo);
}

void Splashscreen::Process(float deltaTime) {
    Control::Process(deltaTime);

    if (m_pAutLogo == nullptr || m_pFmodLogo == nullptr || m_pBackground == nullptr)
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

    m_pAutLogo->m_globalTransform.position.x = m_fWindowWidth / 2 - m_pAutLogo->m_globalTransform.GetWidth() / 2;
    m_pAutLogo->m_globalTransform.position.y = m_fWindowHeight / 2 - m_pAutLogo->m_globalTransform.GetHeight() / 2;
    m_pFmodLogo->m_globalTransform.position.x = m_fWindowWidth / 2 - m_pFmodLogo->m_globalTransform.GetWidth() / 2;
    m_pFmodLogo->m_globalTransform.position.y = m_fWindowHeight / 2 - m_pFmodLogo->m_globalTransform.GetHeight() / 2;
}
