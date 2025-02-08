#include <QtWidgets>

#include "paletterwindow.h"
#include "imageviewer.h"

PaletterGUI::PaletterGUI() : paletterLabel(new QLabel(this))
{
    paletterLabel->setSizePolicy(
        QSizePolicy::Expanding, 
        QSizePolicy::Expanding
    );
    paletterLabel->setAlignment(Qt::AlignCenter);

    const QRect screenGeometry = screen()->geometry();
    
    // how small you can resize the window
    paletterLabel->setMinimumSize(
        screenGeometry.width() / 16, 
        screenGeometry.height() / 16
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(paletterLabel);

    // add the image viewer here

    QHBoxLayout *imgviewerLayout = new QHBoxLayout();
    myImgViewer = new ImageViewer(this);
    imgviewerLayout->addWidget(myImgViewer);
    mainLayout->addLayout(imgviewerLayout);
    
    // add something random to assert my dominance
    QVBoxLayout *buttonsLayout = new QVBoxLayout();
    newLineEdit = new QLineEdit(
        tr("I'm a 10x programmer"), this);
    
    buttonsLayout->addWidget(newLineEdit);
    mainLayout->addLayout(buttonsLayout);

    setWindowTitle(tr("PaletterGUImazing"));
    // this resizing is weird ...
    // why is main window mostly blank space
    resize(400, 400);
}

// resive event override
// this should handle resizing the image
void PaletterGUI::resizeEvent(QResizeEvent *event)
{
    // scale image with window
    myImgViewer->handleResizing();
}

