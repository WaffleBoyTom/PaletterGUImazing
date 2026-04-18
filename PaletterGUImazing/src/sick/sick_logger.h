#ifndef SICK_LOGGER_H
#define SICK_LOGGER_H

#include <QtWidgets>

// determines the color of the
// text being logged
enum class SickLogSeverity 
{
    MSG,          //  white
    SEL,          //  lightseagreen
    CUDA,         //  green
    METAL,        //  light gray
    WARNING,      //  yellow
    ERROR         //  red
};

class SickLogger : public QObject
{
    Q_OBJECT

public:
    static SickLogger *getInstance();

    static void log(QString message, 
                    SickLogSeverity sev = SickLogSeverity::MSG);

signals:
    void tellBossToLog(QString message, SickLogSeverity sev);

private slots:
    void logToQDebug(QString message) const;

private:
    SickLogger();

    static SickLogger *theLogger;
};

#endif  // SICK_LOGGER_H
