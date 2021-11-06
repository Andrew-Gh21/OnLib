#include "OStreamLogger.h"

OStreamLogger::OStreamLogger(std::ostream& stream)
	:stream(stream)
{
}

void OStreamLogger::setPriority(LogSeverity newPriority)
{
	priority = newPriority;
}

void OStreamLogger::Log(const LogMessage& msg, LogSeverity severity)
{
	stream << severityToString[severity] << msg.message.str()<<"\n";
}