#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <functional>

#include <QColor>
#include <QImage>
#include <QList>
#include <QPixmap>

#include "paletterutils.h"

class ImageProcessor
{
public:
    ImageProcessor();

    // removes green component from an image
    // test function you know
    void pixelStuff(QImage &image);

    void fillColorPalette(
        QImage &image, QList<QColor> &palette, 
        const int count
    );

    void applyColorPalette(
        QImage &image, 
        QList<QColor> *palette,
        PaletterUtils::PaletteApplyMode mode,
        PaletterUtils::PaletteProcessorDevice dev    
    );

    void process(
        QImage &image,
        std::function<void(QRgb&)> processor
    );


};

#endif
