#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>
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
		QPixmap getPixmap();
		void setPaletteCount(int count);
	
	private:
		QImage myImage;
		int myPaletteCount;

};

#endif
