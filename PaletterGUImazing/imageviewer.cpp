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

    // pixmap holder -- not a widget
    myImageDisplay = new QPixmap();

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
        tr("I hold the image"),
        this
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

    if (!myImageDisplay->load(*filename))
        return false;
    
    // by default images are pretty big
    // unlike other things...
    // this probably should scale based on
    // main window size
    // dividing by 2 for now, idk
    
    QPixmap scaled = myImageDisplay->scaled(
        myCreator->height() / 2, /* width */
        myCreator->width() / 2, /* height */
        Qt::KeepAspectRatio /* ar */
    );

    // loaded image successfully
    myImageHolder->setPixmap(scaled);

    return true;
            
}

QImage
ImageViewer::getImage()
{

    QPixmap pixmap = myImageHolder->pixmap();
    return pixmap.toImage();

    
}

void
ImageViewer::processImage()
{

    myImageProcessor.loadImage(getImage());
    myImageProcessor.pixelStuff();   
    myImageHolder->setPixmap(
        myImageProcessor.getPixmap()
    );
}
