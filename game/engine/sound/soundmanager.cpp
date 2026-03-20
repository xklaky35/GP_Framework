//
// Created by leon on 20.03.26.
//

#include "soundmanager.h"

#include <cassert>
#include <filesystem>
#include <ranges>
#include <vector>

#include "fmod_errors.h"
#include "fmod_common.h"
#include "../../helper/inlinehelper.h"

#include "../logmanager/logmanager.h"

namespace Engine {

    SoundManager* SoundManager::m_pInstance = nullptr;

    SoundManager::SoundManager() : m_pSystem(nullptr), currentScene(nullptr) {
    }

    SoundManager::~SoundManager() = default;

    SoundManager& SoundManager::GetInstance() {
        if (m_pInstance == nullptr) {
            m_pInstance = new SoundManager();
        }
        return *m_pInstance;
    }

    void SoundManager::DestroyInstance() {
        m_pInstance->m_pSystem->release();
        delete m_pInstance;
        m_pInstance = nullptr;
    }

    bool SoundManager::Initialise() {
        FMOD_RESULT result;

        result = FMOD::System_Create(&m_pSystem);      // Create the main system object.
        if (result != FMOD_OK)
        {
            LogManager::GetInstance().Log(ERROR, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
            return false;
        }

        m_pSystem->setDriver(1);

        result = m_pSystem->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
        if (result != FMOD_OK)
        {
            LogManager::GetInstance().Log(ERROR, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
            return false;
        }
        return true;
    }

    void SoundManager::Process(float deltaTime) {
        if (m_pSystem != nullptr) {
            m_pSystem->update();
        }
    }

    void SoundManager::LoadAudio() {
        for (const auto & entry : std::filesystem::directory_iterator(AUDIO_PATH)) {
            FMOD_RESULT result;
            FMOD::Sound *sound;

            result = m_pSystem->createSound(entry.path().c_str(), FMOD_DEFAULT, nullptr, &sound);

            auto filename = SplitString(entry.path(), "/")[3];

            m_loadedSounds[filename] = sound;
        }
    }

    void SoundManager::Play(const char *soundOnSelection) {
        if (m_loadedSounds.find(soundOnSelection) != m_loadedSounds.end()) {
            FMOD_RESULT result;
            FMOD::Channel *channel;
            result = m_pSystem->playSound(m_loadedSounds[soundOnSelection], nullptr, false, &channel);
            LogManager::GetInstance().Log(INFO,"sound!");
        }
    }
}
