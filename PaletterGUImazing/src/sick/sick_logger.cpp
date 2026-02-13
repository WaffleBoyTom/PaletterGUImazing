#include "sick_logger.h"

SickLogger *
SickLogger::getInstance()
{
    if (theLogger == nullptr)
        theLogger = new SickLogger();
    return theLogger;
}

void
SickLogger::log(QString message, SickLogSeverity sev)
{
    SickLogger *logger = SickLogger::getInstance();
    emit logger->tellBossToLog(std::move(message), sev);
}

SickLogger *SickLogger::theLogger = nullptr;

SickLogger::SickLogger()
{
#ifdef LOG_DEBUG
    connect(this, &SickLogger::tellBossToLog, this, &SickLogger::logToQDebug);
#endif
}

void
SickLogger::logToQDebug(QString message) const
{
    qDebug() << message;
}
