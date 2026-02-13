#include "sick_logger.h"

SickLogger *
SickLogger::getInstance()
{
    if (theLogger == nullptr)
        theLogger = new SickLogger();
    return theLogger;
}

void
SickLogger::log(QString message)
{
    SickLogger *logger = SickLogger::getInstance();
    emit logger->tellBossToLog(std::move(message));
}

SickLogger *SickLogger::theLogger = nullptr;

SickLogger::SickLogger()
{
    connect(this, &SickLogger::tellBossToLog, this, &SickLogger::logToQDebug);
}

void
SickLogger::logToQDebug(QString message) const
{
    qDebug() << message;
}
