#include "cool_logger.h"

CoolLogger *
CoolLogger::getInstance()
{
    if (theLogger == nullptr)
        theLogger = new CoolLogger();
    return theLogger;
}

void
CoolLogger::log(QString message, CoolLogSeverity sev)
{
    CoolLogger *logger = CoolLogger::getInstance();
    emit logger->tellBossToLog(std::move(message), sev);
}

CoolLogger *CoolLogger::theLogger = nullptr;

CoolLogger::CoolLogger()
{
#ifdef LOG_DEBUG
    connect(this, &CoolUtilLogger::tellBossToLog, this, &CoolUtilLogger::logToQDebug);
#endif
}

void
CoolLogger::logToQDebug(QString message) const
{
    qDebug() << message;
}
