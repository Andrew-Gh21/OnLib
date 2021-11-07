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
	LogMessage b;
	b.getTime();

	if (typeid(stream).name() == "St14basic_ofstreamIcSt11char_traitsIcEE")
	{
		stream << b.message.str() << std::setw(12) << severityToString[severity] << " :: " << msg.message.str() << "\n\n";
	}
	else
	{
		static std::mutex logMutex;
		logMutex.lock();
		stream << b.message.str() << std::setw(12) << severityToString[severity] << " :: " << msg.message.str() << "\n\n";
		logMutex.unlock();
	}
	
}
