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



