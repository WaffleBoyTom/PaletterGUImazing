#include "logger.h"

Logger *
Logger::getInstance()
{
    if (theLogger == nullptr)
        theLogger = new Logger();
    return theLogger;
}

void
Logger::log(QString message)
{
    Logger *logger = Logger::getInstance();
    emit logger->tellBossToLog(std::move(message));
}

Logger *Logger::theLogger = nullptr;

Logger::Logger()
{
    connect(this, &Logger::tellBossToLog, this, &Logger::logToQDebug);
}

void
Logger::logToQDebug(QString message) const
{
    qDebug() << message;
}
