
#include "surface.h"

using namespace Engine;

Surface::Surface() {
    SetupNode("Surface", NT_Custom);
}

void Surface::Init() {
    Node::Init();
}

void Surface::Process(float deltaTime) {
    Node::Process(deltaTime);
}

void Surface::SystemProcess() {
    Node::SystemProcess();
}

void Surface::Draw(Renderer &renderer) {
    Node::Draw(renderer);
}

void Surface::DrawDebug() {
    Node::DrawDebug();
}

void Surface::SetupParameter(IniParser *parser, const std::string &sectionId) {
    Node::SetupParameter(parser, sectionId);
    NodeFactory::GetInstance().InitWithConfiguration(this, "../game/scenes/whoosh/Surface.ini");
}
