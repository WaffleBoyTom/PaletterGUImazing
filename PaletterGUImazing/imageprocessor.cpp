#include "imageprocessor.h"

#include <QImage>
#include <QRgb>
#include <QPixmap>

ImageProcessor::ImageProcessor(QImage image)
{
	myImage = image;
}

ImageProcessor::ImageProcessor(){}

void
ImageProcessor::loadImage(QImage image)
{
	myImage = image;
}

void
ImageProcessor::pixelStuff()
{
	// straight up copy from the Qt docs
	// just for testing you know
	// great artists copy ...
	for (int y = 0; y < myImage.height(); ++y) 
	{
    QRgb *line = reinterpret_cast<QRgb*>(myImage.scanLine(y));
    
    for (int x = 0; x < myImage.width(); ++x) 
    {
        QRgb &rgb = line[x];
        rgb = qRgba(
        	qRed(rgb), 
          qGreen(0), 
          qBlue(rgb), 
          qAlpha(rgb)
        );
    }
	}

}

QPixmap
ImageProcessor::getPixmap()
{
	return QPixmap::fromImage(myImage);
}

