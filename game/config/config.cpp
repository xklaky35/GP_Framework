//
// Created by leon on 10.03.26.
//

#include "config.h"

Config* Config::instance = nullptr;

Config & Config::GetInstance() {
    if (instance == nullptr) {
        instance = new Config();
    }
    return *instance;
}

void Config::DestroyInstance() {
    delete instance;
    instance = nullptr;
}

void Config::SetDefaultConfig() {
    windowsHeight = 540;
    windowsWidth = 1080;
}

Config::Config() : windowsWidth(0), windowsHeight(0) {}
Config::~Config() {}
