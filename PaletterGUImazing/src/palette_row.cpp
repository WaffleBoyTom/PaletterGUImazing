#include "palette_row.h"

#include <QtCore/qnamespace.h>

#include <QCursor>
#include <QToolTip>
#include <QtWidgets>

#include "sick_inspector.h"
#include "sick_logger.h"

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
    const int width = this->width() / myBoxCount;
    const int height = this->height();
    const int padding = 10;

    for (int i = 0; i < myBoxCount; ++i)
    {
        const QColor color =
            (i < myPalette->size()) ? myPalette->at(i) : Qt::black;

        switch (myDrawStyle)
        {
        case PaletteDrawStyle::RECT:
        {
            painter.fillRect(
                start + (width * i),  // x
                height / 2,           // y
                width - padding,      // width
                height,               // height
                color
            );
            break;
        }
        case PaletteDrawStyle::APPLE:
        {
            // the reason we set this NoPen
            // is to avoid the circles having
            // a disgusting white outline
            QPen pen;
            pen.setStyle(Qt::NoPen);
            painter.setPen(pen);
            painter.setBrush(color);
            const int rad = qMin((height - padding) / 2, (width - padding) / 2);
            painter.drawEllipse(
                QPoint(
                    (width / 2) + (width * i),
                    height / 2
                ),    // center
                rad,  // rx
                rad   // ry
            );
            break;
        }
        case PaletteDrawStyle::VK:
        {
            // this looks like shit :)
            // no disabling it
            QPoint barycenter((width / 2) + (width * i), height / 2);
            const int rad = qMin((height - padding) / 2, (width - padding) / 2);
            /*
            assuming this is our triangle
                    C

                 A     B

            C is trivial -> barycenter.x, barycenter.y + rad

            A and B can be obtained by using circle
            parametric formula
            angle1 is 210 (240 - 30) as we rotated -30 degrees to
            get our top to be aligned
            angle2 is 330 (360 - 30)

            b.x = center.x + radius * cos(radians(angle1));
            b.y = center.y + radius * sin(radians(angle1));
            a.x = center.x + radius * cos(radians(angle2));
            a.y = center.y + radius * sin(radians(angle2));

            found this out after prototyping in Houdini
            cuz stackoverflow led me astray lol

            */
            QPoint c(barycenter.x(), barycenter.y() + rad);

            QPoint b(
                c.x() + rad * qCos(qDegreesToRadians(210)),
                c.y() + rad * qSin(qDegreesToRadians(210))
            );

            QPoint a(
                c.x() + rad * qCos(qDegreesToRadians(330)),
                c.y() + rad * qSin(qDegreesToRadians(330))
            );

            QPen pen;
            pen.setStyle(Qt::NoPen);
            painter.setPen(pen);
            painter.setBrush(color);

            const int vtx = 3;
            QPoint points[vtx];
            points[0] = c;
            points[1] = b;
            points[2] = a;
            painter.drawPolygon(points, vtx);

            break;
        }
        case PaletteDrawStyle::INVALID:
            break;  // what happened??
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
        // FIXME
        // this 2 should not be hardcoded and
        // should be based on length of enum
        myDrawStyle = PaletteDrawStyle((static_cast<int>(myDrawStyle) + 1) % 2);
        this->repaint();
    }
}

void
PaletteRow::mouseMoveEvent(QMouseEvent *event)
{
    // FIXME ? : why does this not just return black when it fails instead of 
    // returning a std::optional ?
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

    // FIXME
    // this fails when the palette is drawing circles !!!!

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
PaletteRow::serialize(QJsonObject &json, SickExportOpts::ExportFormat fmt)
{
    // assert big time, this should never happen !
    if (!myPalette || myPalette->size() < 2)
        return false;

    for (int i = 0; i < myPalette->size(); ++i)
    {
        QString col_str;
        switch (fmt)
        {
        case SickExportOpts::ExportFormat::HEX:
        {
            col_str = myPalette->at(i).name(QColor::HexRgb);
            break;
        }
        case SickExportOpts::ExportFormat::RGBF:
        {
            float r, g, b;
            myPalette->at(i).getRgbF(&r, &g, &b);
            col_str = QString("%1, %2, %3").arg(r).arg(g).arg(b);
            break;
        }
        case SickExportOpts::ExportFormat::RGBI:
        {
            int r, g, b;
            myPalette->at(i).getRgb(&r, &g, &b);
            col_str = QString("%1, %2, %3").arg(r).arg(g).arg(b);
            break;
        }
        case SickExportOpts::ExportFormat::HSVF:
        {
            float r, g, b;
            myPalette->at(i).getHsvF(&r, &g, &b);
            col_str = QString("%1, %2, %3").arg(r).arg(g).arg(b);
            break;
        }
        case SickExportOpts::ExportFormat::HSVI:
        {
            int r, g, b;
            myPalette->at(i).getHsv(&r, &g, &b);
            col_str = QString("%1, %2, %3").arg(r).arg(g).arg(b);
            break;
        }
        case SickExportOpts::ExportFormat::HSLF:
        {
            float r, g, b;
            myPalette->at(i).getHslF(&r, &g, &b);
            col_str = QString("%1, %2, %3").arg(r).arg(g).arg(b);
            break;
        }
        case SickExportOpts::ExportFormat::HSLI:
        {
            int r, g, b;
            myPalette->at(i).getHsl(&r, &g, &b);
            col_str = QString("%1, %2, %3").arg(r).arg(g).arg(b);
            break;
        }
        case SickExportOpts::ExportFormat::CMYKF:
        {
            float c, m, y, k;
            myPalette->at(i).getCmykF(&c, &m, &y, &k);
            col_str = QString("%1, %2, %3, %4").arg(c).arg(m).arg(y).arg(k);
            break;
        }
        case SickExportOpts::ExportFormat::CMYKI:
        {
            int c, m, y, k;
            myPalette->at(i).getCmyk(&c, &m, &y, &k);
            col_str = QString("%1, %2, %3").arg(c).arg(m).arg(y).arg(k);
            break;
        }
        case SickExportOpts::ExportFormat::INVALID:
            break;
        }

        json.insert(QString("Color %1").arg(i + 1), col_str);
    }
    // sign the json like true gentlemen
    json.insert(QString("Export Format"), SickExportOpts::getFormatToken(fmt));
    json.insert(
        QString("Palette Generator"), QString("PaletterGUImazing 1.0 TM")
    );
    return true;
}
