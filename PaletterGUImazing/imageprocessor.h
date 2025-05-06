#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QColor>
#include <QImage>
#include <QList>
#include <QPixmap>

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
};

#endif
