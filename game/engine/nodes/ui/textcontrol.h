#ifndef GP_FRAMEWORK_TEXTCONTROL_H
#define GP_FRAMEWORK_TEXTCONTROL_H
#include "control.h"


namespace Engine {
    class TextControl : public Control {

    public:
        TextControl();
        void Init() override;
        void Draw(Renderer&) override;
        void SystemProcess() override;
        void SetupParameter(IniParser *parser, const std::string &sectionId) override;
        void SetText(std::string);

        char* GetText();

        Sprite* m_textSprite;
    private:
        bool m_bIsInitialised;
        std::string m_text;
        int m_iPointSize;
        float m_rgba[4];


    };
}



#endif //GP_FRAMEWORK_TEXTCONTROL_H