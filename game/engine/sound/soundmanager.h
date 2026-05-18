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


        void Process(float deltaTime) const;
        FMOD::Channel* PlaySound(const char * sound_on_selection);
        FMOD::Sound* PlayMusic(const char *soundName) const;

    private:
        SoundManager();
        ~SoundManager();

    protected:
        static SoundManager *m_pInstance;

    private:

        FMOD::System* m_pSystem;
        std::pmr::map<std::string, FMOD::Sound*> m_loadedSounds;
        const char* AUDIO_PATH = "../assets/Audio/";
        char* m_pCurrentScene;
    };
}

#endif //GP_FRAMEWORK_SOUNDMANAGER_H