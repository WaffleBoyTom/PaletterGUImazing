#include "imageviewer.h"

#include <QtWidgets>

ImageViewer::ImageViewer(QWidget *parent)
{
    myLayout = new QHBoxLayout(this); 
    myLineEdit = new QLineEdit(tr("Ethan so sexy"), parent);
    myImageDisplay = new QPixmap();
    myLayout->addWidget(myLineEdit);

}
