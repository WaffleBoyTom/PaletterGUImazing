#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <QColor>
#include <QImage>
#include <QList>
#include <QPixmap>
#include <functional>

#include "processor_device.h"
#include "quantizer.h"
#include "remapper.h"

class ImageProcessor
{
public:
    ImageProcessor();

    // removes green component from an image
    // test function you know
    void pixelStuff(QImage &image);

    void fillColorPalette(
        QImage &image, QList<QColor> &palette, const int count
    );

    QVector<QColor> createColorPalette(
        const QImage &image, int palette_size, Quantizer::Method method
    ) const;

    void applyColorPalette(
        QImage &image,
        QList<QColor> *palette,
        PaletteProcessorDevice dev,
        Remapper::CompareMethod method
    );

    void process(QImage &image, std::function<void(QRgb &)> processor);
};

#endif  // IMAGE_PROCESSOR_H
