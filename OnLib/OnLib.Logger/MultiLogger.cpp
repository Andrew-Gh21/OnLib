#include "MultiLogger.h"

void MultiLogger::Send(const LogMessage& msg, LogSeverity severity)const
{
	for (auto element : Loggers)
	{
		element->Log(msg,severity);
	}
}

void MultiLogger::AddLogger(ILogger* logger)
{
	Loggers.push_back(logger);
}
