#include "../config/config.h"
#include "metheorgenerator.h"
#include "meteors.h"

#include <random>

#include "../engine/nodetree/nodetree.h"

MetheorGenerator::MetheorGenerator() : m_meteorCount(0), m_maxMeteorCount(10) {}
MetheorGenerator::~MetheorGenerator() = default;

void MetheorGenerator::Init() {
    Node::Init();
    m_bIsRoot = true;
}

void MetheorGenerator::Process(float deltaTime) {
    Node::Process(deltaTime);
    if (m_meteorCount < m_maxMeteorCount) {
        GenerateMetheor();
    }
}

void MetheorGenerator::Draw(Renderer &renderer) {
    Node::Draw(renderer);
}


void MetheorGenerator::GenerateMetheor() {
    Meteors *m = new Meteors();
    m->SetVelocity(static_cast<float>(GetRandomNumber(1,100)));
    m->SetSpinSpeed(static_cast<float>(GetRandomNumber(1, 20)));
    m->SetSpinDirection(GetRandomNumber(0, 1) == 0 ? 1 : -1);
    m->m_position->x = static_cast<float>(GetRandomNumber(0, Config::GetInstance().windowsWidth));
    m->m_transform->scale = static_cast<float>(GetRandomNumber(1, 3)) / 10;
    m->m_transform->height = 300;
    m->m_transform->width = 300;

    AddChild(*m);

    m_meteorCount++;
}


int MetheorGenerator::GetRandomNumber(int min, int max) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dis(min, max);
    return dis(gen);
}
