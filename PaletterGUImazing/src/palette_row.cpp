#include "palette_row.h"

#include <QCursor>
#include <QToolTip>
#include <QtWidgets>

PaletteRow::PaletteRow(QWidget *parent) : QWidget(parent), myBoxCount(6)
{
    // receive mouseMouseEvent without needing to press a button
    this->setMouseTracking(true);
}

QSize
PaletteRow::sizeHint() const
{
    const int boxWidth = 50;
    const int height = 40;
    return QSize(myBoxCount * boxWidth, height);
}

void
PaletteRow::onPaletteDisplaySizeChanged(int size)
{
    myBoxCount = size;
    this->repaint();
}

void
PaletteRow::onPaletteChanged(QList<QColor> *palette)
{
    myPalette = palette;
    this->repaint();
}

void
PaletteRow::paintEvent(QPaintEvent *event)
{
    if (!myPalette)
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
        const QColor color =
            (i < myPalette->size()) ? myPalette->at(i) : QColor("black");
        painter.fillRect(
            start + (width * i),  // x
            height / 2,           // y
            width - padding,      // width
            height,               // height
            color
        );
    }
}

void
PaletteRow::mouseMoveEvent(QMouseEvent *event)
{
    if (!myPalette)
        return;

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
        QColor color = (index < myPalette->size()) ? myPalette->at(index)
                                                   : QColor("black");
        const QString hex = color.name(QColor::HexRgb);
        QToolTip::showText(event->globalPosition().toPoint(), hex, this);
    }
    else
    {
        QToolTip::hideText();
    }
}
