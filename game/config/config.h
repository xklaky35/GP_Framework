//
// Created by leon on 10.03.26.
//

#ifndef GP_FRAMEWORK_CONFIG_H
#define GP_FRAMEWORK_CONFIG_H


class Config {
public:
    static Config& GetInstance();
    static void DestroyInstance();


    void SetDefaultConfig();


public:
    int windowsWidth;
    int windowsHeight;

protected:
    Config();
    ~Config();
    static Config* instance;
};


#endif //GP_FRAMEWORK_CONFIG_H