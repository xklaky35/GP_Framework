//
// Created by leon on 17.03.26.
//

#include "mainmenu.h"
#include "../../config/config.h"

MainMenu::MainMenu() {
    testBox1 = Control(true);
    testBox2 = Control(true);
    testBox3 = Control(true);
    testBox4 = Control(true);
    testBox5 = Control(true);
    vcontainer1 = VContainer();
    hcontainer2 = HContainer();
    mcontainer1 = MarginContainer();
    panelcontainer1 = PanelContainer();
}

MainMenu::~MainMenu() {

}

void MainMenu::Init() {
    VContainer::Init();

    m_screenSize = Vector2d(Config::GetInstance().windowsWidth, Config::GetInstance().windowsHeight);
    m_globalTransform.SetSize(m_screenSize.x, m_screenSize.y);
    m_controlSpace.x = m_globalTransform.GetWidth();
    m_controlSpace.y = m_globalTransform.GetHeight();



    vcontainer1.AddChild(testBox1);
    vcontainer1.AddChild(testBox4);
    hcontainer2.AddChild(testBox2);
    hcontainer2.AddChild(testBox5);


    AddChild(vcontainer1);
    AddChild(hcontainer2);



}

void MainMenu::Draw(Renderer &renderer) {
    VContainer::Draw(renderer);

    // Load static text textures into the Texture Manager...
    renderer.CreateStaticText("Auckland University of Technology", 50);
    // Generate sprites that use the static text textures...
    m_pWelcomeText = renderer.CreateSprite("Auckland University of Technology");
    m_pWelcomeText->SetY(200);
    m_pWelcomeText->SetX(500);
    /*
    m_pWelcomeText->SetAlpha(1);
    m_pWelcomeText->SetRedTint(0);
    m_pWelcomeText->SetGreenTint(0);
    m_pWelcomeText->SetBlueTint(0);
    */
    m_pWelcomeText->Draw(renderer);
}
