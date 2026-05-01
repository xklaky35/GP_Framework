#include "iniparser.h"

#include <algorithm>
#include <cctype>
#include <fstream>

#include "../logmanager/logmanager.h"
#include "../nodes/node.h"

namespace Engine {

    // ############# Loader ############

    IniParser::IniParser() = default;

    IniParser::~IniParser() = default;

    bool IniParser::LoadIniFile(const std::string &filename) {
        m_filename = filename;
        std::ifstream file(filename);
        if (!file.is_open()) {
            LogManager::GetInstance().Log(WARNING, "Could not find file: %s", filename.c_str());
            return false;
        }

        m_data.clear();

        std::string currentSection;
        std::string line;

        while (std::getline(file, line)) {
            line = Trim(line);

            // Skip blanks and comments (; and #)
            if (line.empty() || line[0] == ';' || line[0] == '#')
                continue;

            // Section header: [SectionName]
            if (line.front() == '[' && line.back() == ']') {
                currentSection = Trim(line.substr(1, line.size() - 2));
                continue;
            }

            // Key = Value
            const auto delimPos = line.find('=');
            if (delimPos == std::string::npos)
                continue; // malformed line – skip

            const std::string key = Trim(line.substr(0, delimPos));
            const std::string value = Trim(line.substr(delimPos + 1));

            if (!key.empty())
                m_data[currentSection][key] = value;
        }

        return true;
    }

    bool IniParser::SaveChangesToIniFile() {
        std::ofstream file(m_filename);
        if (!file.is_open()) {
            LogManager::GetInstance().Log(WARNING, "Could not find file: %s", m_filename.c_str());
            return false;
        }

        for (const auto& [section, keys] : m_data)
        {
            file << "[" << section << "]\n";

            for (const auto& [key, value] : keys)
                file << key << " = " << value << "\n";

            file << "\n";
        }
        return true;
    }

    // ############# Getters #####################

    std::string IniParser::GetValueAsString(const std::string &iniSection, const std::string &key) {
        auto sectionIt = m_data.find(iniSection);
        if (sectionIt == m_data.end())
            return "";

        auto keyIt = sectionIt->second.find(key);
        if (keyIt == sectionIt->second.end())
            return "";

        return keyIt->second;
    }

    int IniParser::GetValueAsInt(const std::string &iniSection, const std::string &key) {
        const std::string value = GetValueAsString(iniSection, key);
        if (value.empty()) return 0;

        try { return std::stoi(value); } catch (...) { return 0; }
    }

    float IniParser::GetValueAsFloat(const std::string &iniSection, const std::string &key) {
        const std::string value = GetValueAsString(iniSection, key);
        if (value.empty()) return 0.0f;

        try { return std::stof(value); } catch (...) { return 0.0f; }
    }

    bool IniParser::GetValueAsBoolean(const std::string &iniSection, const std::string &key) {
        std::string value = GetValueAsString(iniSection, key);

        // Case-insensitive comparison
        std::ranges::transform(value, value.begin(), ::tolower);

        return value == "true" || value == "1" || value == "yes" || value == "on";
    }


    // ################### Setter ######################

    void IniParser::SetValue(const std::string &iniSection, const std::string &key, std::string& value) {
        m_data[iniSection][key] = value;
    }

    void IniParser::SetValue(const std::string &iniSection, const std::string &key, const char* value) {
        m_data[iniSection][key] = value;
    }

    void IniParser::SetValue(const std::string &iniSection, const std::string &key, int value) {
        auto str_value = std::to_string(value);
        m_data[iniSection][key] = str_value;

    }

    void IniParser::SetValue(const std::string &iniSection, const std::string &key, float value) {
        auto str_value = std::to_string(value);
        m_data[iniSection][key] = str_value;

    }

    void IniParser::SetValue(const std::string &iniSection, const std::string &key, bool value) {
        m_data[iniSection][key] = value == true ? "true" : "false";
    }

    // ######### Helper ############

    std::string IniParser::Trim(const std::string &s) {
        const auto begin = s.find_first_not_of(" \t\r\n");
        if (begin == std::string::npos) return "";
        const auto end = s.find_last_not_of(" \t\r\n");
        return s.substr(begin, end - begin + 1);
    }


}
