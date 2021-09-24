
#pragma once

namespace adm
{
	namespace chrono = std::chrono;

	// ============================
	// Timer
	// ============================
	class Timer final
	{
	public:
		enum TimeUnit
		{
			Seconds, Milliseconds, Microseconds
		};

		Timer();
		~Timer() = default;

		// Gets the time that has passed - const version
		float	GetElapsed( const Timer::TimeUnit& tu = Milliseconds ) const;
		// Gets the time that has passed
		float 	GetElapsedAndReset( const Timer::TimeUnit& tu = Milliseconds );
		// Resets the timer
		void 	Reset();

	private:
		chrono::time_point<chrono::system_clock> start;
	};
}
