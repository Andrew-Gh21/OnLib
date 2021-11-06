#pragma once
#include <string>
#include <iostream>
#include <sstream>

enum LogSeverity
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
private:
	static LogSeverity priority;

public:
	virtual void setPriority(LogSeverity newPriority)=0;

	virtual void Log(const LogMessage& msg, LogSeverity sevrity) = 0;
};

template<class T>
inline LogMessage& operator<<(LogMessage& msg, const T& param)
{
	msg.message << param;
	return msg;
}
