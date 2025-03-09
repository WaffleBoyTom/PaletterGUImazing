#include "imageviewer.h"
#include "imageprocessor.h"

#include <QtWidgets>

ImageViewer::ImageViewer(QWidget *parent)
{
    // keep in touch with your parent
    myCreator = parent;

    // main layout
    myLayout = new QVBoxLayout(this); 

    // my boy Ethan so good lookin'
    myLineEdit = new QLineEdit(
        tr("Ethan so sexy"), 
        this
    );

    // nautilus button
    myNautilusButton = new QPushButton(
        tr("Open Image"),  
        this   
    );
    
    connect(
        myNautilusButton, 
        &QPushButton::clicked, 
        this, 
        &ImageViewer::openNautilus
    );

    // image holder
    myImageHolder = new QLabel(
        tr(""),
        this
    );

    myImageHolder->setAlignment(
      Qt::AlignHCenter | Qt::AlignVCenter  
    );

    // processor button
    myProcessorButton = new QPushButton(
        tr("Process Image"),  
        this   
    );
    
    connect(
        myProcessorButton, 
        &QPushButton::clicked, 
        this, 
        &ImageViewer::processImage
    );
    
    // init ImageProcessor
    myImageProcessor = ImageProcessor();

    // populate layout
    myLayout->addWidget(myLineEdit);
    myLayout->addWidget(myNautilusButton);
    myLayout->addWidget(myProcessorButton);
    myLayout->addWidget(myImageHolder);
}

void
ImageViewer::openNautilus()
{
    QString fileName;
    
    // I wonder if /home resolves correctly on windows >?
    // not that I want to support that awful os but 
    // you know...
    
    fileName = QFileDialog::getOpenFileName(
        this,
        tr("Palettize this geezer"), /* title of fileDialog */
        "/home", /* where to start the search */ 
        tr("Image Files (*.png *.jpg)") /* file filter */
    ); 

    // print selected file to console
    qDebug() << fileName;
    if (!loadImage(&fileName))
        // should handle this more gracefully
        qDebug() << "Nope";
    
}

bool
ImageViewer::loadImage(const QString *filename)
{

    myLineEdit->setText(*filename);

    QPixmap image_display;

    if (!image_display.load(*filename))
        return false;
    
    // by default images are pretty big
    // unlike other things...
    // this probably should scale based on
    // main window size
    // dividing by 2 for now, idk
    
    myLoadedImage = image_display;
    
    QPixmap scaled = resizeImage(&image_display);

    // loaded image successfully
    myImageHolder->setPixmap(scaled);

    return true;
            
}

QImage
ImageViewer::getImage()
{
    // we want to load in the original image
    // not the resized one
    return myLoadedImage.toImage();  
}

void
ImageViewer::processImage()
{

    if (myLoadedImage.isNull())
    {
        return;   
    }
    myImageProcessor.loadImage(getImage());
    myImageProcessor.pixelStuff();   
    QPixmap processed = myImageProcessor.getPixmap();
    // override orig with processed to make sure
    // result stays the same when we resize
    myLoadedImage = processed;
    myImageHolder->setPixmap(resizeImage(&processed));
}

QPixmap
ImageViewer::resizeImage(QPixmap *imagedisplay)
{
    // scaling factor hardcoded to 1/2 right now
    // might wanna change that innit
    
    return imagedisplay->scaled(
        myCreator->height() / 2, /* width */
        myCreator->width() / 2, /* height */
        Qt::KeepAspectRatio /* ar */
    );

}

void
ImageViewer::handleResizing()
{   
    if (myLoadedImage.isNull())
        return;
    QPixmap scaled = resizeImage(&myLoadedImage);
    myImageHolder->setPixmap(scaled);
}

void 
ImageViewer::setProcessorPaletteCount(int count)
{
    myImageProcessor.setPaletteCount(count); 
}
