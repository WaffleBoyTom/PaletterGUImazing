#ifndef LOGGER_H
#define LOGGER_H

#include <QtWidgets>

class Logger : public QObject
{
    Q_OBJECT

public:
    static Logger *getInstance();
    static void log(QString message);

signals:
    void tellBossToLog(QString message);

private slots:
    void logToQDebug(QString message) const;

private:
    Logger();

    static Logger *theLogger;
};

#endif  // LOGGER_H
