#ifndef COOL_LOGGER_H
#define COOL_LOGGER_H

#include <QtWidgets>

// determines the color of the
// text being logged
enum class CoolLogSeverity
{
    MSG,      //  white
    SEL,      //  lightseagreen
    CUDA,     //  green
    METAL,    //  light gray
    WARNING,  //  yellow
    ERROR     //  red
};

class CoolLogger : public QObject
{
    Q_OBJECT
public:
    static CoolLogger *getInstance();

    static void log(
        QString message, CoolLogSeverity sev = CoolLogSeverity::MSG
    );

signals:
    void tellBossToLog(QString message, CoolLogSeverity sev);

private slots:
    void logToQDebug(QString message) const;

private:
    CoolLogger();

    static CoolLogger *theLogger;
};

#endif // COOL_LOGGER_H
