#include "OstreamLogger.h"


int main()
{
	int a = 5;
	const std::string name = "John";

	OStreamLogger test(std::cout);
	LogMessage b;
	b << a;
	b << name;

	test.setPriority(LogSeverity::Trace);

	test.Log(b, LogSeverity::Trace);
	test.Log(b, LogSeverity::Debug);
	test.Log(b, LogSeverity::Info);
	test.Log(b, LogSeverity::Warn);
	test.Log(b, LogSeverity::Error);
	test.Log(b, LogSeverity::Crtitical);

	return 0;
}