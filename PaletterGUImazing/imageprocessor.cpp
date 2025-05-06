#include "imageprocessor.h"

#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QRgb>

ImageProcessor::ImageProcessor()
{
}

void
ImageProcessor::pixelStuff(QImage &image)
{
    // straight up copy from the Qt docs
    // just for testing you know
    // great artists copy ...
    // "The computer is the paypah, and Juicetin is the pen. But, we are the
    // hands" - Gods (Gods)
    for (int y = 0; y < image.height(); ++y)
    {
        QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y));
        for (int x = 0; x < image.width(); ++x)
        {
            QRgb &rgb = line[x];
            rgb = qRgba(qRed(rgb), qGreen(0), qBlue(rgb), qAlpha(rgb));
        }
    }
}

void
ImageProcessor::fillColorPalette(
    QImage &image, QList<QColor> &palette, const int count
)
{
    qDebug() << count;
    for (int y = 0; y < count; ++y)
    {
        QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y));
        QColor col = QColor::fromRgb(*line);
        palette.insert(y, col);
    }
}
