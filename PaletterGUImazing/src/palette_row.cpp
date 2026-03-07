#include "palette_row.h"

#include <QtCore/qnamespace.h>

#include <QCursor>
#include <QToolTip>
#include <QtWidgets>

#include "sick_logger.h"
#include "sick_inspector.h"

PaletteRow::PaletteRow(QWidget *parent) : QWidget(parent), myBoxCount(6)
{
    // receive mouseMouseEvent without needing to press a button
    this->setMouseTracking(true);
    setMinimumSize(60, 30);
    myDrawStyle = PaletteDrawStyle::RECT;
}

QSize
PaletteRow::sizeHint() const
{
    const int boxWidth = 50;
    const int height = 50;
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
    painter.setRenderHint(QPainter::Antialiasing, true);
    const int start = 0;

    // the idea behind this padding is that
    // we get a bit of space between each rectangle
    const int width = this->width() / myBoxCount ;
    const int height = this->height();
    const int padding = 10;

    for (int i = 0; i < myBoxCount; ++i)
    {
        const QColor color = (i < myPalette->size()) 
            ? myPalette->at(i) 
            : Qt::black;

        if (myDrawStyle == PaletteDrawStyle::RECT)
        {
            painter.fillRect(
                start + (width * i),  // x
                height / 2,           // y
                width - padding,      // width
                height,               // height
                color
            );
        }
        else if (myDrawStyle == PaletteDrawStyle::APPLE)
        {
            // the reason we set this NoPen
            // is to avoid the circles having
            // a disgusting white outline 
            QPen pen;
            pen.setStyle(Qt::NoPen);
            painter.setPen(pen);
            painter.setBrush(color);
            const int rad = qMin(
                (height - padding) / 2,
                (width - padding) /  2   
            );
            painter.drawEllipse(
                QPoint(
                    (width / 2) + (width * i), 
                    height / 2
                ),         // center
                rad,      // rx
                rad      // ry
            );
        }
    }
}

void
PaletteRow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        std::optional<QColor> color = findColor(event->position().toPoint());
        if (!color)
            return;

        QString name_hex = color->name(QColor::HexRgb);

        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(name_hex);

        SickLogger::log(QString("Copied: %1").arg(name_hex));
    }
    if (event->button() == Qt::MouseButton::RightButton)
    {
        //TODO: add context menu !!
        // but maybe not ..
        myDrawStyle = PaletteDrawStyle(
            1 - static_cast<int>(myDrawStyle)    
        );
        this->repaint();
    }
}

void
PaletteRow::mouseMoveEvent(QMouseEvent *event)
{
    std::optional<QColor> color = findColor(event->position().toPoint());

    if (color)
    {
        SickInspector::displayColor(*color, event->globalPosition().toPoint());
    }
    else
    {
        SickInspector::displayColor(
            Qt::black, event->globalPosition().toPoint()
        );
    }
}

void
PaletteRow::leaveEvent(QEvent *event)
{
    SickInspector::hideColor();
}

std::optional<QColor>
PaletteRow::findColor(QPoint position) const
{
    if (!myPalette)
        return std::nullopt;

    const int width = this->width() / myBoxCount;
    const int height = this->height();
    const int padding = 10;

    const int x = static_cast<int>(position.x());
    const int y = static_cast<int>(position.y());

    const int index = x / width;

    const int x_low = width * index;
    const int x_high = x_low + width - padding;
    const bool x_check = x_low <= x && x <= x_high;

    const int y_low = height / 2;
    const int y_high = y_low + height;
    const bool y_check = y_low <= y && y <= y_high;

    if (0 <= index && index < myPalette->size() && x_check && y_check)
        return myPalette->at(index);
    return std::nullopt;
}

bool
PaletteRow::serialize(QJsonObject &json)
{

    // assert big time, this should never happen !
    if (!myPalette || myPalette->size() < 2)
        return false;

    for (int i = 0; i < myPalette->size(); ++i)
    {
        QString name_hex = myPalette->at(i).name(QColor::HexRgb);
        json.insert(QString("Color %1").arg(i + 1), name_hex);
    }
    // sign the json like true gentlemen
    json.insert(
        QString("Palette Generator"),
        QString("PaletterGUImazing 1.0 TM")  
    );
    return true;

}
