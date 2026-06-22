#include "baller_image_processor.h"

#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QRgb>
#include <QtMath>

#include "nerd_types.h"
#include "palette_generator.h"
#include "remapper.h"

BallerImageProcessor::BallerImageProcessor()
{
}

void
BallerImageProcessor::pixelStuff(QImage &image)
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
BallerImageProcessor::fillColorPalette(
    QImage &image, QList<QColor> &palette, const int count
)
{
    qDebug() << "color palette count = " << count;
    for (int y = 0; y < count; ++y)
    {
        QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y));
        QColor col = QColor::fromRgb(*line);
        palette.insert(y, col);
    }
}

QVector<QColor>
BallerImageProcessor::createColorPalette(
    const QImage &image,
    int palette_size,
    NerdPaletteAlgorithm algorithm
) const
{
    return PaletteGenerator(palette_size, algorithm).generatePalette(image);
}

void
BallerImageProcessor::applyColorPalette(
    QImage &image,
    QList<QColor> *palette,
    BallerDevice dev,
    NerdCompareMethod method
) const
{
    if (palette == nullptr)
        return;

    Remapper remapper(method, *palette);

    // stop going through the palette if we're within .05
    switch (dev)
    {
    case BallerDevice::CPU:
    {
        remapper.remapHost(image);
        break;
    }
    case BallerDevice::GPU:
    {
        remapper.remap(image);
        break;
    }
    }
}

void
BallerImageProcessor::process(QImage &image, std::function<void(QRgb &)> processor)
{
    for (int y = 0; y < image.height(); ++y)
    {
        QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y));

        for (int x = 0; x < image.width(); ++x)
        {
            QRgb &rgb = line[x];

            processor(rgb);
        }
    }
}
