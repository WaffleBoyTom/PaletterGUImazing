#include "imageviewer.h"

#include <QtWidgets>

ImageViewer::ImageViewer(QWidget *parent)
{
    myLayout = new QHBoxLayout(this); 
    myLineEdit = new QLineEdit(
        tr("Ethan so sexy"), 
        this
    );
    myImageDisplay = new QPixmap();
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
    
    myLayout->addWidget(myLineEdit);
    myLayout->addWidget(myNautilusButton);

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
    myLineEdit->setText(fileName);
}
