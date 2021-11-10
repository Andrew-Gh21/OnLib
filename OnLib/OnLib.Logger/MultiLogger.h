#pragma once
#include "ILogger.h"

class MultiLogger
{
private:
	std::vector<ILogger*> Loggers;

public:
	MultiLogger() = default;
	MultiLogger(const MultiLogger& aux) = default;
	~MultiLogger() = default;

	void Send(const LogMessage& msg , LogSeverity severity)const;

	void AddLogger(ILogger* logger);
};

