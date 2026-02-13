#ifndef SICK_LOG_VIEWER_H
#define SICK_LOG_VIEWER_H

#include <QtWidgets>
#include "sick_logger.h"


class SickLogViewer : public QPlainTextEdit
{
    Q_OBJECT

public:
    SickLogViewer(QWidget *parent);

public slots:
    void append(QString message);
    void appendLine(QString message, 
                    SickLogSeverity sev = SickLogSeverity::MSG);

private:
    QString mContents;
};

#endif  // SICK_LOG_VIEWER_H
