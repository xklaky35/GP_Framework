// COMP710 GP Framework
// This include:
#include "logmanager.h"
// Library includes:
#include <cstdarg>
#include <cstdio>

// Static Members:
LogManager *LogManager::sm_pInstance = nullptr;

LogManager &LogManager::GetInstance() {
    if (sm_pInstance == 0) {
        sm_pInstance = new LogManager();
    }
    return (*sm_pInstance);
}

void LogManager::DestroyInstance
() {
    delete sm_pInstance;
    sm_pInstance = nullptr;
}

LogManager::LogManager() = default;
LogManager::~LogManager() = default;

void LogManager::Log(const GP_LOG_LEVEL level, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    switch (level) {
        case CRITICAL: {
            printf("\033[31mCRITICAL: \033[0m");
            break;
        }
        case ERROR: {
            printf("\033[31mERROR: \033[0m");
            break;
        }
        case WARNING: {
            printf("\033[33mWARNING: \033[0m");
            break;
        }
        case INFO: {
            printf("\033[34mINFO: \033[0m");
            break;
        }
        case DEBUG: {
            printf("DEBUG:");
            break;
        }
        default: {}
    }
    vprintf(fmt, args);
    puts("");

    va_end(args);
}

void LogManager::LogError(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    Log(ERROR, fmt, args);
    va_end(args);
}
