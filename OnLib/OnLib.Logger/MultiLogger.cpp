#include "MultiLogger.h"

void MultiLogger::Send(const LogMessage& msg, LogSeverity severity)const
{
	for (const auto& element : loggers)
	{
		element->Log(msg,severity);
	}
}

void MultiLogger::AddLogger(std::shared_ptr<ILogger> logger)
{
	loggers.push_back(logger);
}
