#include "OStreamLogger.h"

OStreamLogger::OStreamLogger(std::ostream& stream)
	:stream(stream)
{
}

void OStreamLogger::SetPriority(LogSeverity newPriority)
{
	priority = newPriority;
}

void OStreamLogger::Log(const LogMessage& msg, LogSeverity severity)
{
	static std::mutex logMutex;
	LogMessage datetime;
	datetime.GetTime();

	logMutex.lock();
	stream << datetime.message.str() << std::setw(12) << severityToString[severity] << " :: " << msg.message.str() << "\n\n";
	logMutex.unlock();
	
}
