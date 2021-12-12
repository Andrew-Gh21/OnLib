#pragma once
#include <ostream>
#include "ILogger.h"

class OStreamLogger : public ILogger
{
private:
    std::ostream& stream;
   
public:
    OStreamLogger(std::ostream& stream);

    void Log(const LogMessage& msg, LogSeverity sevrity)override;

};
