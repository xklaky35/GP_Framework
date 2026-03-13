#ifndef GP_FRAMEWORK_INPUT_H
#define GP_FRAMEWORK_INPUT_H

#include <map>
#include "SDL.h"

namespace Engine {
    class Input {
    public:
        static Input &GetCurrentEvents();
        static void DestroyInstance();
        void RegisterEvent(SDL_Event&);

        bool IsPressed(SDL_Keycode);


    private:
        Input();
        ~Input();
        SDL_Event* m_pCurrentEvent;
        static Input *m_pInstance;
        std::map<SDL_Keycode, bool> m_pressedKeys;
    };

}


#endif //GP_FRAMEWORK_INPUT_H
