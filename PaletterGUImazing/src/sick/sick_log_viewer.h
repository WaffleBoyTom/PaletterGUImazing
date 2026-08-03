#ifndef SICK_LOG_VIEWER_H
#define SICK_LOG_VIEWER_H

#include <QtWidgets>

#include "cool_logger.h"

class SickLogViewer : public QPlainTextEdit
{
    Q_OBJECT

public:
    SickLogViewer(QWidget *parent);

public slots:
    void append(QString message);
    void appendLine(
        QString message, CoolLogSeverity sev = CoolLogSeverity::MSG
    );

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QString myContents;
    QPoint myStartDragPos;
};

#endif  // SICK_LOG_VIEWER_H
