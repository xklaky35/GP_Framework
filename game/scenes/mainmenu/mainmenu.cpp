#include "mainmenu.h"
#include "../../config/config.h"
#include "../../engine/game.h"
#include "../../engine/input/input.h"
#include "../../engine/logmanager/logmanager.h"
#include "../../engine/nodes/nodefactory.h"
#include "../../engine/scenemanager/scenemanager.h"
#include "../../engine/sound/soundmanager.h"

MainMenu::MainMenu() : m_bSelectionLocked(false), m_currentSelection(0), m_selectionParents{},
                       m_sceneOptions{"Whoosh", "Quit"} {
    vcontainer1 = new VContainer();
    h1 = new HContainer();
    h2 = new HContainer();
    h3 = new HContainer();
    h4 = new HContainer();
    t1 = new TextControl();
    t2 = new TextControl();
    spacer1 = new Control();
    spacer2 = new Control();
    spacer3 = new Control();
    spacer4 = new Control();
    cursor = new TextControl();
    SetupNode("MainMenu", NT_Custom);
}

void MainMenu::Init() {
    HContainer::Init();


    /* The Main menu proveds a choice between the 3 different games to play
     * The Games are interactivaly selectable by moving the cursor up and down and pressing enter once a game is selected
     *
     * >  Play SpaceInvaders
     *    Play Robotron
     *    Play Asteroids
     *
     *         Quit
     */


    // OUTER LAYOUT
    m_screenSize = Vector2d(Config::GetInstance().windowsWidth, Config::GetInstance().windowsHeight);
    m_globalTransform.SetSize(m_screenSize.x, m_screenSize.y);
    m_controlSpace.x = m_globalTransform.GetWidth();
    m_controlSpace.y = m_globalTransform.GetHeight();
    m_containerSizing.m_bExpandHorizontal = true;
    m_containerSizing.m_bExpandVertical = true;


    // Spacer config
    spacer1->m_containerSizing.m_bExpandHorizontal = true;
    spacer1->m_containerSizing.m_bExpandVertical = true;
    spacer2->m_containerSizing.m_bExpandHorizontal = true;
    spacer2->m_containerSizing.m_bExpandVertical = true;
    spacer3->m_containerSizing.m_bExpandHorizontal = true;
    spacer3->m_containerSizing.m_bExpandVertical = true;
    spacer4->m_containerSizing.m_bExpandHorizontal = true;
    spacer4->m_containerSizing.m_bExpandVertical = true;

    vcontainer1->m_containerSizing.m_bExpandHorizontal = true;
    vcontainer1->m_containerSizing.m_bExpandVertical = true;


    // CENTER LAYOUT

    h1->m_containerSizing.m_bExpandHorizontal = true;
    h1->m_containerSizing.m_bExpandVertical = true;
    h2->m_containerSizing.m_bExpandHorizontal = true;
    h2->m_containerSizing.m_bExpandVertical = true;
    h3->m_containerSizing.m_bExpandHorizontal = true;
    h3->m_containerSizing.m_bExpandVertical = true;
    h4->m_containerSizing.m_bExpandHorizontal = true;
    h4->m_containerSizing.m_bExpandVertical = true;


    // ROW LAYOUT

    t1->m_containerSizing.m_bExpandHorizontal = true;
    t1->m_containerSizing.m_bExpandVertical = true;
    t2->m_containerSizing.m_bExpandHorizontal = true;
    t2->m_containerSizing.m_bExpandVertical = true;



    t1->m_containerSizing.m_verticalBehavior = v_Center;
    t2->m_containerSizing.m_verticalBehavior = v_Center;
    t1->m_containerSizing.m_horizontalBehavior = h_Center;
    t2->m_containerSizing.m_horizontalBehavior = h_Center;

    t1->SetText("Play");
    t2->SetText("Quit");


    cursor->m_containerSizing.m_bExpandVertical = true;
    cursor->m_containerSizing.m_verticalBehavior = v_Center;
    cursor->SetText(">");


    // Add Text to rows
    m_selectionParents[0] = h1;
    h1->AddChild(*cursor);
    h1->AddChild(*t1);
    m_selectionParents[1] = h2;
    h2->AddChild(*t2);



    // Add rows to center column
    vcontainer1->AddChild(*spacer1);
    vcontainer1->AddChild(*h1);
    vcontainer1->AddChild(*h2);
    vcontainer1->AddChild(*spacer2);


    AddChild(*spacer3);
    AddChild(*vcontainer1);
    AddChild(*spacer4);

    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/mainmenu/MainMenu.ini");
}


void MainMenu::Process(float deltaTime) {
    HContainer::Process(deltaTime);
    const char* soundOnSelection = "hardpop-mainmenu-onSelection.wav";
    HandleMouseInteraction();

    if (InputManager::GetInstance().GetButtonState(SDLK_DOWN)) {
        if (!m_bSelectionLocked) {
            if (m_currentSelection < 1) {
                SoundManager::GetInstance().Play(soundOnSelection);
                m_currentSelection++;
            }
            m_bSelectionLocked = true;
        }
    }
    else if (InputManager::GetInstance().GetButtonState(SDLK_UP)) {
        if (!m_bSelectionLocked) {
            if (m_currentSelection > 0) {
                SoundManager::GetInstance().Play(soundOnSelection);
                m_currentSelection--;
            }
            m_bSelectionLocked = true;
        }
    }
    else if (InputManager::GetInstance().GetButtonState(SDLK_RETURN)) {
        if (!m_bSelectionLocked) {
            ExecuteOption();
            return;
        }
    }
    else {
        m_bSelectionLocked = false;
    }
    cursor->m_parent = m_selectionParents[m_currentSelection];

    if (t1->m_textSprite != nullptr && t2->m_textSprite != nullptr && cursor->m_textSprite != nullptr) {
        t1->m_textSprite->m_iLayer = 1;
        t2->m_textSprite->m_iLayer = 1;
        cursor->m_textSprite->m_iLayer = 1;
    }

}

void MainMenu::SetupParameter(IniParser *parser, const std::string &sectionId) {
    HContainer::SetupParameter(parser, sectionId);
}

void MainMenu::Draw(Renderer &renderer) {
    HContainer::Draw(renderer);
}

void MainMenu::HandleMouseInteraction() {
    auto mousePos = InputManager::GetInstance().GetMousePosition();
    auto mouseEvent = InputManager::GetInstance().GetCurrentMouseEvent();


    if (mousePos.y < h2->GetGlobalPosition().y) {
        m_currentSelection = 0;
    }
    if (mousePos.y > h2->GetGlobalPosition().y) {
        m_currentSelection = 1;
    }

    if (mouseEvent.type == SDL_MOUSEBUTTONDOWN && mouseEvent.button == 1) {
        ExecuteOption();
    }
    LogManager::GetInstance().Log(INFO, "%f, %f", mousePos.x, mousePos.y);

}

void MainMenu::ExecuteOption() {
    if (m_currentSelection == 0)
        SceneManager::GetInstance().SetSceneActive("Whoosh");
    if (m_currentSelection == 1)
        Game::GetInstance().Quit();
}
