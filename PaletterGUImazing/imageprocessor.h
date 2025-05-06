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
    ImageProcessor(QImage image);

    void loadImage(QImage image);

    // removes green component from an image
    // test function you know
    void pixelStuff();

    void fillColorPalette(QList<QColor> &palette, int count);

    QPixmap getPixmap();

private:
    // I don't think this should hold on to an image
    // it should probably just get passed an image by reference
    // or hold a pointer to the image ?
    QImage myImage;
};

#endif
