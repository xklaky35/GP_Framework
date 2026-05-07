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
    float GetTotalTime()  const;

private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    Timer()
    {
        Reset();
    }

    TimePoint m_StartTime;
    TimePoint m_LastTime;
    float     m_DeltaTime  = 0.0f;
    float     m_TotalTime  = 0.0f;
    uint64_t  m_FrameCount = 0;


    static Timer* m_pInstance;
};


#endif //GP_FRAMEWORK_TIMER_H