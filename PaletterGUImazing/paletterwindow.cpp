#include <QtWidgets>

#include "paletterwindow.h"


PaletterGUI::PaletterGUI() : paletterLabel(new QLabel(this))
{
    paletterLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    paletterLabel->setAlignment(Qt::AlignCenter);

    const QRect screenGeometry = screen()->geometry();
    paletterLabel->setMinimumSize(screenGeometry.width() / 8, screenGeometry.height() / 8);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(paletterLabel);

    // testing adding a button and connecting an action to it
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    newTestButton = new QPushButton(tr("Open Image to Palette"), this);
    connect(newTestButton, &QPushButton::clicked, this, &PaletterGUI::buttonTestAction);
    buttonsLayout->addWidget(newTestButton);
    mainLayout->addLayout(buttonsLayout);

    setWindowTitle(tr("PaletterGUImazing"));
    resize(300, 200);
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
