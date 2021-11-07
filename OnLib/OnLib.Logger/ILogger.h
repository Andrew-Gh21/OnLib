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
	void getTime()
	{
		std::time_t currentTime = std::time(0);
		std::tm* timestamp = std::localtime(&currentTime);
		char buffer[40];
		strftime(buffer, 40, "%d/%m/%g %T", timestamp);
		message << buffer<< " ";
	}
};


class ILogger
{
protected:
	LogSeverity priority;
	std::string fileName;
	std::unordered_map<LogSeverity, std::string> severityToString;

public:

	ILogger();

	virtual void setPriority(LogSeverity newPriority) = 0;

	virtual void Log(const LogMessage& msg, LogSeverity sevrity) = 0;
};

template<class T>
inline LogMessage& operator<<(LogMessage& msg, const T& param)
{
	msg.message << param;
	return msg;
}