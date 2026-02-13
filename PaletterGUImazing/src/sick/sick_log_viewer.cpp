#include "sick_log_viewer.h"
#include <QtCore/qassert.h>

SickLogViewer::SickLogViewer(QWidget *parent) : QPlainTextEdit(parent)
{
    setBackgroundVisible(true);
    setReadOnly(true);
    setWordWrapMode(QTextOption::WrapMode::NoWrap);
    setPlaceholderText("Log is empty.");
    setFixedHeight(150);

    setStyleSheet("QPlainTextEdit { background-color: black; color: white; }");
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
    cursor.movePosition(QTextCursor::End); // Move cursor to the end

    // Apply the character format (color)
    QTextCharFormat format;
    switch (sev)
    {
        case SickLogSeverity::MSG:
        {
            format.setForeground(QBrush(Qt::white));
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

    // Insert the text and a newline character to form a line
    cursor.insertText(message);
    cursor.insertText("\n");
}
