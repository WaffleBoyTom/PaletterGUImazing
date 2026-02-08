#include "image_processor.h"

#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QRgb>
#include <QtMath>
#include <functional>

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
    qDebug() << "color palette count = " << count;
    for (int y = 0; y < count; ++y)
    {
        QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y));
        QColor col = QColor::fromRgb(*line);
        palette.insert(y, col);
    }
}

QVector<QColor>
ImageProcessor::createColorPalette(
    const QImage &image, int palette_size, Quantizer::Method method
) const
{
    return Quantizer(palette_size, method).generatePalette(image);
}

void
ImageProcessor::applyColorPalette(
    QImage &image,
    QList<QColor> *palette,
    PaletteProcessorDevice dev,
    Remapper::CompareMethod method
)
{
    // TODO:
    // add handling by mode
    // multithread this >?
    // run this on the GuhPoo with CUDA (or metal :> )
    // turn this loop bs into a lambda

    if (palette == nullptr)
        return;

    Remapper remapper(method, *palette);

    // stop going through the palette if we're within .05
    switch (dev)
    {
    case PaletteProcessorDevice::CPU:
    {
        remapper.remapHost(image);
        break;
    }
    case PaletteProcessorDevice::GPU:
    {
        remapper.remap(image);
        break;
    }
    }
}

void
ImageProcessor::process(QImage &image, std::function<void(QRgb &)> processor)
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
