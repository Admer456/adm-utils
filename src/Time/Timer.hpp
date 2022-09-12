
#pragma once

namespace adm
{
	namespace chrono = std::chrono;

	struct TimeUnits
	{
		enum Type
		{
			Seconds, Milliseconds, Microseconds, Nanoseconds
		};
	};

	
	template<typename Scalar, typename Clock>
	class TimerTemplate final
	{
	public:
		TimerTemplate()
		{
			Reset();
		}
		~TimerTemplate() = default;

		// Gets the time that has passed - const version
		inline Scalar GetElapsed( const TimeUnits::Type& tu = TimeUnits::Milliseconds ) const
		{
			Scalar elapsed = Scalar( 0 );

			const auto now = Clock::now();
			const auto nanos = Scalar( chrono::duration_cast<chrono::nanoseconds>( now - start ).count() );
		
			switch ( tu )
			{
			case TimeUnits::Seconds:      return nanos / Scalar( 1000000000 );
			case TimeUnits::Milliseconds: return nanos / Scalar( 1000000 );
			case TimeUnits::Microseconds: return nanos / Scalar( 1000 );
			case TimeUnits::Nanoseconds:  return nanos;
			}

			return Scalar( 0 );
		}
		
		// Gets the time that has passed
		inline Scalar GetElapsedAndReset( const TimeUnits::Type& tu = TimeUnits::Milliseconds )
		{
			Scalar elapsed = GetElapsed( tu );

			Reset();

			return elapsed;
		}

		// Resets the timer
		inline void Reset()
		{
			start = Clock::now();
		}

	private:
		chrono::time_point<Clock> start;
	};

	// ============================
	// Timer
	// 
	// A class you can use to measure time intervals:
	// Timer t;
	// SomeExpensiveFunction();
	// float milliseconds = t.GetElapsed();
	// ============================
	using Timer =              TimerTemplate<float,  chrono::steady_clock>;
	// Measures system time, don't use this for profiling
	using TimerSystem =        TimerTemplate<float,  chrono::system_clock>;
	// Highest precision clock timer
	using TimerPrecise =       TimerTemplate<float,  chrono::high_resolution_clock>;

	using TimerDouble =        TimerTemplate<double, chrono::steady_clock>;
	using TimerSystemDouble =  TimerTemplate<double, chrono::system_clock>;
	using TimerPreciseDouble = TimerTemplate<double, chrono::high_resolution_clock>;
}
