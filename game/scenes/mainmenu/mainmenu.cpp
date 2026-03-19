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
    textcontrol = TextControl();
}

MainMenu::~MainMenu() {

}

void MainMenu::Init() {
    HContainer::Init();

    // menu container setup
    m_screenSize = Vector2d(Config::GetInstance().windowsWidth, Config::GetInstance().windowsHeight);
    m_globalTransform.SetSize(m_screenSize.x, m_screenSize.y);
    m_controlSpace.x = m_globalTransform.GetWidth();
    m_controlSpace.y = m_globalTransform.GetHeight();

    //

    testBox1.AddChild(testBox5);
    vcontainer1.AddChild(testBox1);
    vcontainer1.AddChild(testBox2);
    hcontainer2.AddChild(testBox3);
    hcontainer2.AddChild(testBox4);

    textcontrol.SetText("SPACEINVADER");
    panelcontainer1.AddChild(textcontrol);

    AddChild(vcontainer1);
    AddChild(hcontainer2);
    AddChild(panelcontainer1);

}

void MainMenu::Draw(Renderer &renderer) {
    HContainer::Draw(renderer);


}
