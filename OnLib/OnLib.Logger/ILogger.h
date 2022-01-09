#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <mutex>
#include <sstream>
#include <string>
#include <unordered_map>

enum class LogSeverity
{
	Trace,
	Debug,
	Info,
	Warn,
	Error,
	Crititical
};

struct LogMessage
{
	std::stringstream message;
	template<class T>
	friend LogMessage& operator << (LogMessage& msg, const T& param);
	static std::string GetTime()
	{
		std::time_t currentTime = std::time(0);
		std::tm* timestamp=new std::tm;
		localtime_s(timestamp, &currentTime);
		char buffer[40];
		strftime(buffer, 40, "%d/%m/%g %T", timestamp);
		return std::string(buffer);
	}
};


class ILogger
{
protected:
	LogSeverity priority;
	static std::unordered_map<LogSeverity, std::string> severityToString;

public:

	ILogger();
	ILogger(const ILogger& aux) = default;
	ILogger& operator=(const ILogger& aux) = default;

	ILogger(ILogger&& aux) = default;
	ILogger& operator=(ILogger&& aux) = default;

	~ILogger() = default;

	void SetPriority(LogSeverity newPriority);

	virtual void Log(const LogMessage& msg, LogSeverity sevrity) = 0;
};

template<class T>
inline LogMessage& operator<<(LogMessage& msg, const T& param)
{
	msg.message << param;
	return msg;
}