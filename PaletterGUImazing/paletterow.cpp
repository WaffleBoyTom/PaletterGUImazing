#include "paletterow.h"

#include <QCursor>
#include <QToolTip>
#include <QtWidgets>

PaletteRow::PaletteRow(QWidget *parent) : myBoxCount(6)
{
    // keep in touch with your parent
    myCreator = parent;

    // receive mouseMouseEvent without needing to press a button
    this->setMouseTracking(true);
}

void
PaletteRow::onPaletteCountChanged(const int count)
{
    myBoxCount = count;
    this->repaint();
}

void
PaletteRow::drawPalette(QList<QColor> *paletteptr)
{
    myPalettePtr = paletteptr;
    this->repaint();
}

void
PaletteRow::paintEvent(QPaintEvent *event)
{
    if (!myPalettePtr)
        return;

    QPainter painter(this);
    const int start = 0;

    // the idea behind this padding is that
    // we get a bit of space between each rectangle
    const int width = this->width() / myBoxCount;
    const int height = this->height();
    const int padding = 10;

    for (int i = 0; i < myBoxCount; ++i)
    {
        const int index = qBound(0, i, 50);
        painter.fillRect(
            start + (width * i),  // x
            height / 2,           // y
            width - padding,      // width
            height,               // height
            myPalettePtr->at(index)
        );
    }
}

void
PaletteRow::mouseMoveEvent(QMouseEvent *event)
{
    const int width = this->width() / myBoxCount;
    const int height = this->height();
    const int padding = 10;

    const int x = static_cast<int>(event->position().x());
    const int y = static_cast<int>(event->position().y());

    const int index = x / width;

    const int x_low = width * index;
    const int x_high = x_low + width - padding;
    const bool x_check = x_low <= x && x <= x_high;

    const int y_low = height / 2;
    const int y_high = y_low + height;
    const bool y_check = y_low <= y && y <= y_high;

    if (index >= 0 && x_check && y_check)
    {
        const QColor &color = myPalettePtr->at(index);
        const QString hex = color.name(QColor::HexRgb);
        QToolTip::showText(event->globalPosition().toPoint(), hex, this);
    }
    else
    {
        QToolTip::hideText();
    }
}
