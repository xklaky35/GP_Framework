#include "soundmanager.h"
#include <cassert>
#include <filesystem>
#include <vector>
#include "fmod_errors.h"
#include "fmod_common.h"
#include "../../helper/inlinehelper.h"
#include "../logmanager/logmanager.h"

namespace Engine {

    SoundManager* SoundManager::m_pInstance = nullptr;

    SoundManager::SoundManager() : m_pSystem(nullptr), m_pCurrentScene(nullptr) {
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
        FMOD_RESULT result = FMOD::System_Create(&m_pSystem);      // Create the main system object.
        if (result != FMOD_OK)
        {
            LogManager::GetInstance().Log(ERROR, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
            return false;
        }

        m_pSystem->setDriver(1);

        result = m_pSystem->init(512, FMOD_INIT_NORMAL, nullptr);    // Initialize FMOD.
        if (result != FMOD_OK)
        {
            LogManager::GetInstance().Log(ERROR, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
            return false;
        }

        return true;
    }

    void SoundManager::Process(float deltaTime) const {
        if (m_pSystem != nullptr) {
            m_pSystem->update();
        }
    }

    void SoundManager::LoadAudio() {
        for (const auto & entry : std::filesystem::directory_iterator(AUDIO_PATH)) {
            FMOD::Sound *sound;

            FMOD_RESULT result = m_pSystem->createSound(entry.path().c_str(), FMOD_DEFAULT, nullptr, &sound);

            auto filenameArray = SplitString(entry.path(), "/");
            auto &filename = filenameArray[filenameArray.size()-1];

            m_loadedSounds[filename] = sound;
        }
    }
    FMOD::Sound* SoundManager::PlayMusic(const char *soundName) const {
        if (m_loadedSounds.contains(soundName)) {
            FMOD::Sound *sound;
            FMOD_RESULT result = m_pSystem->createStream(soundName, FMOD_NONBLOCKING, nullptr, &sound);
            return sound;
        }
        return nullptr;
    }

    FMOD::Channel* SoundManager::PlaySound(const char *soundName) {
        if (m_loadedSounds.contains(soundName)) {
            FMOD::Channel *channel;
            FMOD_RESULT result = m_pSystem->playSound(m_loadedSounds[soundName], nullptr, false, &channel);
            return channel;
        }
        return nullptr;
    }
}
