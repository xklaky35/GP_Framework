//
// Created by leon on 20.03.26.
//

#ifndef GP_FRAMEWORK_SOUNDMANAGER_H
#define GP_FRAMEWORK_SOUNDMANAGER_H
#include <map>
#include <string>

#include "fmod.hpp"


namespace Engine {
    class SoundManager {
    public:
        static SoundManager &GetInstance();
        static void DestroyInstance();

        bool Initialise();
        void LoadAudio();
        void Process(float deltaTime);
        void Play(const char * sound_on_selection);

    private:
        SoundManager();
        ~SoundManager();

    protected:
        static SoundManager *m_pInstance;

    private:

        FMOD::System* m_pSystem;
        std::pmr::map<std::string, FMOD::Sound*> m_loadedSounds;
        const char* AUDIO_PATH = "../assets/Audio/";
        char* currentScene;
    };
}

#endif //GP_FRAMEWORK_SOUNDMANAGER_H