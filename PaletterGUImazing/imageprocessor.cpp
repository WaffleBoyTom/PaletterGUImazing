#include "imageprocessor.h"

#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QRgb>
#include <QtMath>

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

void 
ImageProcessor::applyColorPalette(QImage &image, QList<QColor>* palette)
{

    // stop going through the palette if we're within .05
    float threshold = .05;
    // FIXME: This shit is so fucked...
    
    for (int y = 0; y < image.height(); ++y)
    {
        QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y));
        for (int x = 0; x < image.width(); ++x)
        {
            QRgb &rgb = line[x];
            
            float paletter, paletteg, paletteb; 

            float delta = 1000.0;
            
            for (int i = 0; i < palette->size(); ++i)

            {
                QColor currcolor = QColor(rgb);
                palette->at(i).getRgbF(&paletter, &paletteg, &paletteb);
                float length_delta = qSqrt(
                    qPow(paletter - currcolor.redF(), 2) +
                    qPow(paletteg - currcolor.greenF(), 2) +
                    qPow(paletteb - currcolor.blueF(), 2)    
                );

                if (length_delta < delta)
                {
                    rgb = qRgba(
                        int(paletter * 255), 
                        int(paletteg * 255), 
                        int(paletteb * 255), 
                        255
                    );    
                }
                if (length_delta < threshold)
                    break; // optimization
            }
            
            // rgb = qRgba(qRed(rgb), qGreen(0), qBlue(rgb), qAlpha(rgb));
        }
    }
    
}
