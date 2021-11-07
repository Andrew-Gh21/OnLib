#pragma once
#include <ostream>
#include "ILogger.h"

class OStreamLogger : public ILogger
{
private:
    std::ostream& stream;
   
public:
    OStreamLogger(std::ostream& stream);

    void setPriority(LogSeverity newPriority)override;

    void Log(const LogMessage& msg, LogSeverity sevrity)override;
};
