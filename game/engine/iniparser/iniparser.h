#ifndef GP_FRAMEWORK_INIPARSER_H
#define GP_FRAMEWORK_INIPARSER_H
#include <cstring>
#include <string>
#include <unordered_map>

typedef  std::unordered_map<std::string, std::unordered_map<std::string, std::string>> NodeConfiguration;

namespace Engine {
    class Node;
    class IniParser {
    public:

        IniParser();
        ~IniParser();


        bool LoadIniFile(const std::string &filename);
        bool SaveChangesToIniFile();

        std::string GetValueAsString(const std::string &iniSection, const std::string &key);
        int GetValueAsInt(const std::string &iniSection, const std::string &key);
        float GetValueAsFloat(const std::string &iniSection, const std::string &key);
        bool GetValueAsBoolean(const std::string &iniSection, const std::string &key);

        void SetValue(const std::string &iniSection, const std::string &key, std::string &value);
        void SetValue(const std::string &iniSection, const std::string &key, const char *value);
        void SetValue(const std::string &iniSection, const std::string &key, int value);
        void SetValue(const std::string &iniSection, const std::string &key, float value);
        void SetValue(const std::string &iniSection, const std::string &key, bool value);
        static int GetIndexOf(const char *arr[] , const char * str, const int length);


        NodeConfiguration m_data;
    private:
        std::string m_filename;
        static std::string Trim(const std::string &s);
    };
}

#endif