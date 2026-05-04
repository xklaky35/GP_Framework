#ifndef GP_FRAMEWORK_INPUT_H
#define GP_FRAMEWORK_INPUT_H

#include <map>
#include <queue>

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

    struct KeyState {
        SDL_Keycode code;
        ButtonState state;
    };

    class XboxController;

    class InputManager {
    public:
        static InputManager &GetInstance();
        static void DestroyInstance();
        void RegisterEvent(SDL_Event&);
        ButtonState GetButtonState(SDL_Keycode);

        Vector2d GetMousePosition() const;

        SDL_MouseButtonEvent GetCurrentMouseEvent() const;

        void Process(float delta_time);

    protected:
        Vector2d m_mousePosition;
        Vector2d m_mouseWheel;
        bool m_bRelativeMouseMode;
        XboxController* m_pXboxController;
        int m_iNumAttachedControllers;



    private:
        InputManager();
        ~InputManager();
        static InputManager *m_pInstance;

        SDL_Keycode m_previousKey;
        std::map<SDL_Keycode, ButtonState> m_pressedKeys;
        std::queue<KeyState> m_isInInstableState;


        SDL_MouseButtonEvent m_currentMouseEvent;

    };

}


#endif //GP_FRAMEWORK_INPUT_H
