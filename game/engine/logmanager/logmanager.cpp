#include "logmanager.h"

#include <cstdarg>
#include <cstdio>

namespace Engine {
    // Static Members:
    LogManager *LogManager::s_pInstance = nullptr;

    LogManager::LogManager() = default;
    LogManager::~LogManager() = default;

    LogManager &LogManager::GetInstance() {
        if (s_pInstance == nullptr) {
            s_pInstance = new LogManager();
        }
        return (*s_pInstance);
    }

    void LogManager::DestroyInstance() {
        delete s_pInstance;
        s_pInstance = nullptr;
    }


    void LogManager::Log(const GP_LOG_LEVEL level, const char *fmt, ...) {
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
            default: {
            }
        }
        vprintf(fmt, args);
        puts("");

        va_end(args);
    }

    void LogManager::LogError(const char *fmt, ...) {
        va_list args;
        va_start(args, fmt);
        Log(ERROR, fmt, args);
        va_end(args);
    }
}
