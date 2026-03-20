#ifndef GP_FRAMEWORK_INPUT_H
#define GP_FRAMEWORK_INPUT_H

#include <map>
#include "SDL.h"
#include "../structs/vector2d.h"

namespace Engine {
    enum ButtonState
    {
        BS_NEUTRAL,
        BS_PRESSED,
        BS_RELEASED,
        BS_HELD
    };
    class XboxController;

    class InputManager {
    public:
        static InputManager &GetCurrentEvents();
        static void DestroyInstance();
        void RegisterEvent(SDL_Event&);
        bool GetButtonState(SDL_Keycode);


    protected:
        Vector2d m_mousePosition;
        Vector2d m_mouseWheel;
        unsigned int m_previousMouseButtons;
        unsigned int m_currentMouseButtons;
        bool m_bRelativeMouseMode;
        XboxController* m_pXboxController;
        int m_iNumAttachedControllers;

        //################################




    private:
        InputManager();
        ~InputManager();
        SDL_Event* m_pCurrentEvent;
        static InputManager *m_pInstance;
        std::map<SDL_Keycode, bool> m_pressedKeys;
    };

}


#endif //GP_FRAMEWORK_INPUT_H
