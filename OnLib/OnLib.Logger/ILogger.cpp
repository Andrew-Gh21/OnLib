#include "ILogger.h"

ILogger::ILogger()
{
	severityToString[LogSeverity::Trace] = "[Trace]\t";
	severityToString[LogSeverity::Debug]= "[Debug]\t";
	severityToString[LogSeverity::Info]= "[Info]\t";
	severityToString[LogSeverity::Warn]= "[Warn]\t";
	severityToString[LogSeverity::Error]= "[Error]\t";
	severityToString[LogSeverity::Crtitical]= "[Crtitical]\t";
}
