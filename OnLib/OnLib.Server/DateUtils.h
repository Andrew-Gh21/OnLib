#pragma once
#include <ctime>
#include <string>

class DateUtils
{
public:
	static std::tm GetTimeFromString(const std::string& dateString);
	static std::string TimeToString(const std::string& format, const std::tm& time);
	static std::tm AddDays(std::tm& time, int days);
	static std::tm GetCurrentLocalTime();
	static int GetDaysBetween(const std::tm& first, const std::tm& second);
	static double DaysToSeconds(int days);
};

