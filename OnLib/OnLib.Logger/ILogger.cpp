#include "ILogger.h"

std::unordered_map<LogSeverity, std::string> ILogger::severityToString =
{
	{LogSeverity::Trace,"[Trace]"},
	{LogSeverity::Debug,"[Debug]"},
	{LogSeverity::Info,"[Info]"},
	{LogSeverity::Warn,"[Warn]"},
	{LogSeverity::Error,"[Error]"},
	{LogSeverity::Crititical,"[Crititical]"}
};

ILogger::ILogger()
{
	priority = LogSeverity::Trace;
}

void ILogger::SetPriority(LogSeverity newPriority)
{
	priority = newPriority;
}
