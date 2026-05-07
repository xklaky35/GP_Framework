#include "Level1.h"

#include "../../../engine/nodes/nodefactory.h"
#include "../../../engine/time/timer.h"

Level1::Level1() {
    SetupNode("Level1", NT_Custom);
    m_timerDisplay = nullptr;
}

void Level1::Init() {
    Node::Init();
    m_timerDisplay = dynamic_cast<TextControl *>(GetChild("LevelTimer"));
    Timer::GetInstance().Reset();
}

void Level1::Process(float deltaTime) {
    Node::Process(deltaTime);
    return;

    // need to find another way
    // performance in butchered doint this
    if (m_timerDisplay != nullptr) {
        auto secondsRaw = Timer::GetInstance().GetTotalTime();
        auto seconds = ((int)secondsRaw % 60);
        auto minutes = (int)(secondsRaw/60);

        std::string secondsText = std::to_string(seconds);
        std::string minutesText = std::to_string(minutes);
        //m_timerDisplay->SetText(minutesText + " : " + secondsText);
    }
}

void Level1::SetupParameter(IniParser *parser, const std::string &sectionId) {
    Node::SetupParameter(parser, sectionId);
    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/whoosh/Level1/Level1.ini");
}

