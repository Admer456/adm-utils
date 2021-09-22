
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

		// Gets the time that has passed
		float 	GetElapsed( const TimeUnit& tu = Milliseconds, const bool& reset = true );
		// Resets the timer
		void 	Reset();

	private:
		chrono::time_point<chrono::system_clock> start;
	};
}
