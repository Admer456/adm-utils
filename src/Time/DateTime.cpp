
#include "../Precompiled.hpp"
#include "date/date.h"

using namespace adm;

using SystemTimePoint = chrono::system_clock::time_point;
using SystemTimeDuration = chrono::duration<chrono::system_clock::rep, chrono::system_clock::period>;

// ============================
// FromYMDandHMS
// ============================
inline DateTime FromYMDandHMS( const date::year_month_day& ymd, const date::hh_mm_ss<chrono::seconds>& hms )
{
	return DateTime::FromFullDate(
		int( ymd.year() ),
		unsigned int( ymd.month() ),
		unsigned int( ymd.day() ),
		hms.hours().count(),
		hms.minutes().count(),
		hms.seconds().count() );
}

// ============================
// DateTime::Now
// ============================
DateTime DateTime::Now()
{
	const SystemTimePoint tpNow = chrono::system_clock::now();
	const auto days = date::floor<date::days>( tpNow );
	const date::year_month_day ymd{ days };

	// Visual Studio IntelliSense will most likely break here, but don't worry,
	// it is perfectly valid code, it just requires a lot of auto deduction
	const SystemTimeDuration difference = tpNow - days;
	const auto seconds = chrono::floor<chrono::seconds>( difference );
	const date::hh_mm_ss hms{ seconds };

	return FromYMDandHMS( ymd, hms );
}

// ============================
// DateTime::FromDuration
// ============================
DateTime DateTime::FromDuration( float duration, const TimeUnits::Type& tu )
{
	size_t seconds = duration;
	switch ( tu )
	{
	case TimeUnits::Milliseconds: seconds /= 1'000; break;
	case TimeUnits::Microseconds: seconds /= 1'000'000; break;
	case TimeUnits::Nanoseconds:  seconds /= 1'000'000'000; break;
	}

	const SystemTimePoint chronoSeconds = SystemTimePoint( chrono::seconds( seconds ) );
	const auto days = date::floor<date::days>( chronoSeconds );
	const date::year_month_day ymd{ days };

	const SystemTimeDuration difference = chronoSeconds - days;
	const auto leftoverSeconds = chrono::floor<chrono::seconds>( difference );
	const date::hh_mm_ss hms{ leftoverSeconds };

	return FromYMDandHMS( ymd, hms );
}

// ============================
// DateTime::FromHMS
// ============================
DateTime DateTime::FromHMS( int hour, int minute, int second )
{
	DateTime date;
	date.hour = hour;
	date.minute = minute;
	date.second = second;
	return date;
}

// ============================
// DateTime::FromYMD
// ============================
DateTime DateTime::FromYMD( int year, int month, int day )
{
	DateTime date;
	date.year = year;
	date.month = month;
	date.day = day;
	return date;
}

// ============================
// DateTime::FromFullDate
// ============================
DateTime DateTime::FromFullDate( int year, int month, int day, int hour, int minute, int second )
{
	DateTime date;
	date.year = year;
	date.month = month;
	date.day = day;
	date.hour = hour;
	date.minute = minute;
	date.second = second;
	return date;
}
