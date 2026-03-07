#include "sick_log_viewer.h"
#include <QtCore/qassert.h>

SickLogViewer::SickLogViewer(QWidget *parent) : QPlainTextEdit(parent)
{
    setBackgroundVisible(true);
    setReadOnly(true);
    setWordWrapMode(QTextOption::WrapMode::NoWrap);
    setPlaceholderText("Log is empty.");
    setFixedHeight(150);

    setStyleSheet("QPlainTextEdit { background-color: black }");
    setMaximumBlockCount(1000);
}

void
SickLogViewer::append(QString message)
{
    moveCursor(QTextCursor::MoveOperation::Start);
    insertPlainText(message);
}

void
SickLogViewer::appendLine(QString message, SickLogSeverity sev)
{
    QTextCursor cursor(textCursor());
    cursor.movePosition(QTextCursor::End);

    QTextCharFormat format;
    switch (sev)
    {
        case SickLogSeverity::MSG:
        {
            // Qt::cyan looks pretty cool
            // maybe we could give the user an option
            // to set the messages to cyan ...
            format.setForeground(QBrush(Qt::white));
            break;
        }
        case SickLogSeverity::CUDA:
        {
            format.setForeground(QBrush(Qt::green));
            break;
        }
        case SickLogSeverity::METAL:
        {
            format.setForeground(QBrush(Qt::lightGray));
            break;
        }
        case SickLogSeverity::WARNING:
        {
            format.setForeground(QBrush(Qt::yellow));
            break;
        }
        case SickLogSeverity::ERROR:
        {
            format.setForeground(QBrush(Qt::red));
            break;
        }
        Q_ASSERT("How did we get here !!");
        format.setForeground(QBrush(Qt::white));
        break;
    }
    
    cursor.setCharFormat(format);
    cursor.insertText(message);
    cursor.insertText("\n");
}
void 
SickLogViewer::mousePressEvent(QMouseEvent *event) 
{
    if (event->button() == Qt::LeftButton) 
        myStartDragPos = event->pos();
}

void 
SickLogViewer::mouseMoveEvent(QMouseEvent *event) 
{
    if (!(event->buttons() & Qt::LeftButton)) 
        return;

    
    const QPoint pos = myStartDragPos - event->pos();
    // make sure we're at least 150 tall
    setFixedHeight(qMax(150, height() + pos.y()));
    
    event->accept();
}

void 
SickLogViewer::mouseReleaseEvent(QMouseEvent *event) 
{
}
