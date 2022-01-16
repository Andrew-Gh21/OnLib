#include "DateUtils.h"

std::tm DateUtils::GetTimeFromString(const std::string& dateString)
{
	std::tm date{};
	date.tm_year = std::stoi(dateString.substr(6, 2)) + 100;
	date.tm_mon = std::stoi(dateString.substr(3, 2)) - 1;
	date.tm_mday = std::stoi(dateString.substr(0, 2));

	return date;
}

std::string DateUtils::TimeToString(const std::string& format, const std::tm& time)
{
	char buffer[40];
	std::strftime(buffer, 40, format.c_str(), &time);
	return std::string(buffer);
}

std::tm DateUtils::AddDays(std::tm& time, int days)
{
	std::tm finalTime{};
	time_t added = std::mktime(&time) + DaysToSeconds(days);
	localtime_s(&finalTime, &added);

	return finalTime;
}

std::tm DateUtils::GetCurrentLocalTime()
{
	auto timeSec = std::time(nullptr);
	std::tm time{};

	localtime_s(&time, &timeSec);
	return time;
}

int DateUtils::GetDaysBetween(const std::tm& first, const std::tm& second)
{
	return std::abs(std::difftime(std::mktime((std::tm*)&first), std::mktime((std::tm*)&second)) / (60.0 * 60.0 * 24.0));
}

double DateUtils::DaysToSeconds(int days)
{
	return (double)days * 24.0 * 60.0 * 60.0;
}
