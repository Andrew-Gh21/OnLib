#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <mutex>

enum class LogSeverity
{
	Trace,
	Debug,
	Info,
	Warn,
	Error,
	Crtitical
};

struct LogMessage
{
	std::stringstream message;
	template<class T>
	friend LogMessage& operator << (LogMessage& msg, const T& param);
};


class ILogger
{
protected:
	LogSeverity priority;
	std::unordered_map<LogSeverity, std::string> severityToString;

public:

	ILogger();

	virtual void setPriority(LogSeverity newPriority) = 0;

	virtual void Log(const LogMessage& msg, LogSeverity sevrity) = 0;
};

template<class T>
inline LogMessage& operator<<(LogMessage& msg, const T& param)
{
	msg.message << param <<" ";
	return msg;
}