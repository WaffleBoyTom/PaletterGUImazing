#include <QtWidgets>

#include "paletterwindow.h"
#include "imageviewer.h"

PaletterGUI::PaletterGUI() : paletterLabel(new QLabel(this))
{
    paletterLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    paletterLabel->setAlignment(Qt::AlignCenter);

    const QRect screenGeometry = screen()->geometry();
    paletterLabel->setMinimumSize(screenGeometry.width() / 8, screenGeometry.height() / 8);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(paletterLabel);

    // try adding the image viewer here

    QHBoxLayout *imgviewerLayout = new QHBoxLayout();
    myImgViewer = new ImageViewer(this);
    imgviewerLayout->addWidget(myImgViewer);
    mainLayout->addLayout(imgviewerLayout);
    
    QVBoxLayout *buttonsLayout = new QVBoxLayout();
    newTestButton = new QPushButton(tr("Open Image to Palette"), this);
    newLineEdit = new QLineEdit(tr("I'm a 10x programmer"), this);
    connect(newTestButton, &QPushButton::clicked, this, &PaletterGUI::buttonTestAction);
    
    buttonsLayout->addWidget(newTestButton);
    buttonsLayout->addWidget(newLineEdit);
    mainLayout->addLayout(buttonsLayout);

    setWindowTitle(tr("PaletterGUImazing"));
    resize(600, 300);
}

// resive event override
// void PaletterGUI::resizeEvent(QResizeEvent * /* event */)
// {
// }

void 
PaletterGUI::buttonTestAction()
{
    QString fileName;
    
    fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open Image to Palette"), /* title of fileDialog */
        "/home", /* where to start the search */ 
        tr("Image Files (*.png *.jpg)") /* file filter */
    ); 

    // print selected file to console
    qDebug() << fileName;
}
