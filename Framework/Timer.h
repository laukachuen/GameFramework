#pragma once
#include <chrono>

class Timer
{
public:
    Timer() noexcept;

    // Returns time passed (in seconds) since last call to Mark()
    float Mark() noexcept;

    // Returns total time passed (in seconds) since timer started
    float Peek() const noexcept;

private:
    std::chrono::steady_clock::time_point m_last;
};