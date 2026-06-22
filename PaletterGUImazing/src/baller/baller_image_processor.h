#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <functional>

#include <QColor>
#include <QImage>
#include <QList>
#include <QPixmap>

#include "baller_types.h"
#include "nerd_types.h"


// TODO: UNUSED
class BallerImageProcessor
{
public:
    BallerImageProcessor();

    // removes green component from an image
    // test function you know
    void pixelStuff(QImage &image);

    void fillColorPalette(
        QImage &image, QList<QColor> &palette, const int count
    );

    QVector<QColor> createColorPalette(
        const QImage &image,
        int palette_size,
        NerdPaletteAlgorithm algorithm
    ) const;

    void applyColorPalette(
        QImage &image,
        QList<QColor> *palette,
        BallerDevice dev,
        NerdCompareMethod method
    ) const;

    void process(QImage &image, std::function<void(QRgb &)> processor);
};

#endif  // IMAGE_PROCESSOR_H
