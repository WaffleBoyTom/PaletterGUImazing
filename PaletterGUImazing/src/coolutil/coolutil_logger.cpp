#include "coolutil_logger.h"

CoolUtilLogger *
CoolUtilLogger::getInstance()
{
    if (theLogger == nullptr)
        theLogger = new CoolUtilLogger();
    return theLogger;
}

void
CoolUtilLogger::log(QString message, CoolUtilLogSeverity sev)
{
    CoolUtilLogger *logger = CoolUtilLogger::getInstance();
    emit logger->tellBossToLog(std::move(message), sev);
}

CoolUtilLogger *CoolUtilLogger::theLogger = nullptr;

CoolUtilLogger::CoolUtilLogger()
{
#ifdef LOG_DEBUG
    connect(this, &CoolUtilLogger::tellBossToLog, this, &CoolUtilLogger::logToQDebug);
#endif
}

void
CoolUtilLogger::logToQDebug(QString message) const
{
    qDebug() << message;
}
