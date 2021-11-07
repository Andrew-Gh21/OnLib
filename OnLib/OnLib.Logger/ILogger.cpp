#include "ILogger.h"

ILogger::ILogger()
{
	severityToString[LogSeverity::Trace] = "[Trace]";
	severityToString[LogSeverity::Debug]= "[Debug]";
	severityToString[LogSeverity::Info]= "[Info]";
	severityToString[LogSeverity::Warn]= "[Warn]";
	severityToString[LogSeverity::Error]= "[Error]";
	severityToString[LogSeverity::Crititical]= "[Crititical]";

	fileName = "log.txt";
}
