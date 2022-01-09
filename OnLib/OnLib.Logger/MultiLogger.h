#pragma once
#include "ILogger.h"
#include <memory>

class MultiLogger
{
private:
	std::vector<std::shared_ptr<ILogger>> loggers;

public:
	MultiLogger() = default;

	MultiLogger(const MultiLogger& aux) = default;
	MultiLogger& operator=(const MultiLogger& aux) = default;

	MultiLogger(MultiLogger&& aux) = default;
	MultiLogger& operator=(MultiLogger&& aux) = default;

	~MultiLogger() = default;

	void Send(const LogMessage& msg , LogSeverity severity)const;

	void AddLogger(std::shared_ptr<ILogger> logger);
};

