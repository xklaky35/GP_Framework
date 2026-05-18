// COMP710 GP Framework 2025
#ifndef LOGMANAGER_H
#define LOGMANAGER_H

namespace Engine {
    enum GP_LOG_LEVEL {
        CRITICAL = 1,
        ERROR,
        WARNING,
        INFO,
        DEBUG,
    };

    class LogManager {
        // Member methods:
    public:
        static LogManager &GetInstance();
        static void DestroyInstance();
        void Log(GP_LOG_LEVEL, const char *, ...);
        void LogError(const char *fmt, ...);

    private:
        LogManager();
        ~LogManager();

        // Member data:
    protected:
        static LogManager *s_pInstance;

    private:
    };

}
#endif
