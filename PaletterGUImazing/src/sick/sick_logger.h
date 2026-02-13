#ifndef SICK_LOGGER_H
#define SICK_LOGGER_H

#include <QtWidgets>

class SickLogger : public QObject
{
    Q_OBJECT

public:
    static SickLogger *getInstance();
    static void log(QString message);

signals:
    void tellBossToLog(QString message);

private slots:
    void logToQDebug(QString message) const;

private:
    SickLogger();

    static SickLogger *theLogger;
};

#endif  // SICK_LOGGER_H
