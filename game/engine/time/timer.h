#ifndef GP_FRAMEWORK_TIMER_H
#define GP_FRAMEWORK_TIMER_H

#include <chrono>

class Timer
{
public:
    static Timer& GetInstance();
    static void DestroyInstance();

    void Tick();
    void Reset();

    // Seconds since Reset()
    [[nodiscard]] float GetTotalTime()  const;
    [[nodiscard]] std::string GetTotalTimeAsString() const;

private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    Timer();


    TimePoint m_StartTime;
    TimePoint m_LastTime;
    float     m_DeltaTime;
    float     m_TotalTime;
    uint64_t  m_FrameCount;


    static Timer* m_pInstance;
};


#endif //GP_FRAMEWORK_TIMER_H