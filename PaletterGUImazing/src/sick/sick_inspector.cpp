#include "sick_inspector.h"

#include <QtGui/qbrush.h>

void
SickInspector::displayColor(QColor color, QPoint position)
{
    SickInspector *inspector = SickInspector::getInstance();

    inspector->myColor = color;
    inspector->move(position + QPoint(16, 16));
    inspector->show();
    inspector->repaint();
}

void
SickInspector::hideColor()
{
    SickInspector *inspector = SickInspector::getInstance();

    inspector->hide();
}

void
SickInspector::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QBrush brush(Qt::black);
    painter.setBrush(brush);

    QPen pen(Qt::white);
    pen.setWidth(1);
    painter.setPen(pen);

    QRect r = rect().adjusted(1, 1, -1, -1);
    painter.drawRect(r);

    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);

    const QString hex_name = myColor.name(QColor::HexRgb);
    painter.drawText(QPoint(10, 30), hex_name);

    const QString rgb_name = QString("(%1,%2,%3)")
                                 .arg(myColor.red())
                                 .arg(myColor.green())
                                 .arg(myColor.blue());
    painter.drawText(QPoint(10, 50), rgb_name);

    brush.setColor(myColor);
    painter.setBrush(brush);

    painter.drawRect(QRect(90, 12, 50, 50));
}

SickInspector *SickInspector::theInspector = nullptr;

SickInspector *
SickInspector::getInstance()
{
    if (SickInspector::theInspector == nullptr)
        SickInspector::theInspector = new SickInspector();
    return SickInspector::theInspector;
}

SickInspector::SickInspector() : QWidget(nullptr)
{
    setWindowFlags(
        Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
        Qt::WindowDoesNotAcceptFocus | Qt::WindowTransparentForInput
    );
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
    raise();

    setFixedSize(QSize(150, 75));
}
