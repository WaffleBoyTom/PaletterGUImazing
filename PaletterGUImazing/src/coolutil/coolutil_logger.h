#ifndef COOLUTIL_LOGGER_H
#define COOLUTIL_LOGGER_H

#include <QtWidgets>

// determines the color of the
// text being logged
enum class CoolUtilLogSeverity
{
    MSG,      //  white
    SEL,      //  lightseagreen
    CUDA,     //  green
    METAL,    //  light gray
    WARNING,  //  yellow
    ERROR     //  red
};

class CoolUtilLogger : public QObject
{
    Q_OBJECT
public:
    static CoolUtilLogger *getInstance();

    static void log(
        QString message, CoolUtilLogSeverity sev = CoolUtilLogSeverity::MSG
    );

signals:
    void tellBossToLog(QString message, CoolUtilLogSeverity sev);

private slots:
    void logToQDebug(QString message) const;

private:
    CoolUtilLogger();

    static CoolUtilLogger *theLogger;
};

#endif // COOLUTIL_LOGGER_H
