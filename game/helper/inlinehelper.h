#include <random>



inline int GetRandomInt(const int min, const int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dis(min, max);
    return dis(gen);
}

inline signed char GetRandomPosOrNeg() {
    return GetRandomInt(0,1) == 0 ? -1 : 1;
}

inline float GetRandomPercentage() {
    return static_cast<float>(GetRandomInt(0,100)) / 100.f;
}

inline std::vector<std::string> SplitString(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

inline std::string GenerateUID() {

    const char* v = "0123456789abcdef";
    const bool dash[] = {0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0};

    std::string res;
    for (bool i : dash) {
        if (i) res+= "-";
        res += v[GetRandomInt(0,15)];
        res += v[GetRandomInt(0,15)];
    }
    return res;
}



