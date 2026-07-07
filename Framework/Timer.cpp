#include "Timer.h"

Timer::Timer() noexcept
{
    m_last = std::chrono::steady_clock::now();
}

float Timer::Mark() noexcept
{
    const auto old = m_last;
    m_last = std::chrono::steady_clock::now();
    const std::chrono::duration<float> frameTime = m_last - old;
    return frameTime.count();
}

float Timer::Peek() const noexcept
{
    return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_last).count();
}