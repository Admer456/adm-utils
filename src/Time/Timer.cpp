
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
float Timer::GetElapsed( const TimeUnit& tu, const bool& reset )
{
    float elapsed = 0.0f;

    const auto now = chrono::system_clock::now();
    const float seconds = chrono::duration<float>( now - start ).count();

    switch ( tu )
    {
        case Timer::Seconds: elapsed = seconds; break;
        case Timer::Milliseconds: elapsed = seconds * 1000.0f; break;
        case Timer::Microseconds: elapsed = seconds * 1000000.0f; break;
    }

    if ( reset )
    {
        Reset();
    }

    return elapsed;
}

// ============================
// Timer::Reset
// ============================
void Timer::Reset()
{
    start = chrono::system_clock::now();
}
