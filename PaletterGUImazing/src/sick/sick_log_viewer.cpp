#include "sick_log_viewer.h"

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
SickLogViewer::appendLine(QString message)
{
    moveCursor(QTextCursor::MoveOperation::Start);
    insertPlainText(QString("%1\n").arg(message));
}
