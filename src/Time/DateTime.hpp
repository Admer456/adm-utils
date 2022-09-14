
#pragma once

namespace adm
{
	class DateTime final
	{
	public:
		DateTime() = default;

		static DateTime Now();
		static DateTime FromDuration( float duration, const TimeUnits::Type& tu = TimeUnits::Seconds );
		static DateTime FromHMS( int hour = 0, int minute = 0, int second = 0 );
		static DateTime FromYMD( int year = 0, int month = 0, int day = 0 );
		static DateTime FromFullDate( int year = 0, int month = 0, int day = 0,
			int hour = 0, int minute = 0, int second = 0 );

		const int& GetYear() const { return year; }
		const int& GetMonth() const { return month; }
		const int& GetDay() const { return day; }
		const int& GetHour() const { return hour; }
		const int& GetMinute() const { return minute; }
		const int& GetSecond() const { return second; }

	private:
		int year{};
		int month{};
		int day{};
		int hour{};
		int minute{};
		int second{};
	};
}
