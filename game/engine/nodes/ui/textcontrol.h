#ifndef GP_FRAMEWORK_TEXTCONTROL_H
#define GP_FRAMEWORK_TEXTCONTROL_H
#include "control.h"


namespace Engine {
    class TextControl : public Control {

    public:
        TextControl();
        ~TextControl() override;
        void Init() override;
        void Draw(Renderer&) override;
        void SystemProcess() override;
        void SetText(std::string);

    private:
        bool m_bIsInitialised;
        std::string m_text;
        int m_iPointSize;
        Sprite* m_textSprite;
        float m_rgba[4];


    };
}



#endif //GP_FRAMEWORK_TEXTCONTROL_H