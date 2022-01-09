#pragma once
#include <ostream>
#include "ILogger.h"

class OStreamLogger : public ILogger
{
private:
    std::ostream& stream;
   
public:
    OStreamLogger(std::ostream& stream);
	OStreamLogger() = default;

	OStreamLogger(const OStreamLogger& aux) = default;
	OStreamLogger& operator=(const OStreamLogger& aux) = default;

	OStreamLogger(OStreamLogger&& aux) = default;
	OStreamLogger& operator=(OStreamLogger&& aux) = default;

	~OStreamLogger() = default;

    void Log(const LogMessage& msg, LogSeverity sevrity)override;

};
