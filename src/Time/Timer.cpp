
#include "Precompiled.hpp"
using namespace adm;

// ============================
// Timer::ctor
// ============================
Timer::Timer()
{
    Reset();
}

// ============================
// Timer::GetElapsed
// ============================
float Timer::GetElapsed( const TimeUnit& tu ) const
{
    float elapsed = 0.0f;

    const auto now = chrono::system_clock::now();
    const auto nanos = chrono::duration_cast<chrono::nanoseconds>(now - start).count();
    
    switch ( tu )
    {
    case Timer::Seconds:      elapsed = nanos / 1000000000.0f; break;
    case Timer::Milliseconds: elapsed = nanos / 1000000.0f; break;
    case Timer::Microseconds: elapsed = nanos / 1000.0f; break;
    case Timer::Nanoseconds:  elapsed = nanos; break;
    }

    return elapsed;
}

// ============================
// Timer::GetElapsedAndReset
// ============================
float Timer::GetElapsedAndReset( const TimeUnit& tu )
{
    float elapsed = GetElapsed( tu );

    Reset();

    return elapsed;
}

// ============================
// Timer::Reset
// ============================
void Timer::Reset()
{
    start = chrono::system_clock::now();
}
