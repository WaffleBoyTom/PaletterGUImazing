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

    // add the image viewer here

    QHBoxLayout *imgviewerLayout = new QHBoxLayout();
    myImgViewer = new ImageViewer(this);
    imgviewerLayout->addWidget(myImgViewer);
    mainLayout->addLayout(imgviewerLayout);
    
    // add something random to assert my dominance
    QVBoxLayout *buttonsLayout = new QVBoxLayout();
    newLineEdit = new QLineEdit(tr("I'm a 10x programmer"), this);
    buttonsLayout->addWidget(newLineEdit);
    mainLayout->addLayout(buttonsLayout);

    setWindowTitle(tr("PaletterGUImazing"));
    resize(600, 300);
}

// resive event override
// void PaletterGUI::resizeEvent(QResizeEvent * /* event */)
// {
// }

