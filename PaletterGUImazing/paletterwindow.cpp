#include "paletterwindow.h"

#include <QtWidgets>

#include "imageviewer.h"
#include "paletteviewer.h"

PaletterGUI::PaletterGUI() : paletterLabel(new QLabel(this))
{
    const QRect screenGeometry = screen()->geometry();
    const QSize screenSize = screen()->size();
    // how small you can resize the window
    paletterLabel->setMinimumSize(
        screenGeometry.width() / 16, screenGeometry.height() / 16
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

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

    QVBoxLayout *paletteviewerlayout = new QVBoxLayout();
    // add the paletteviewer
    myPaletteViewer = new PaletteViewer(this);
    // when the slider is changed, set the palette count
    // on the image processor
    connect(
        myPaletteViewer,
        &PaletteViewer::tellBossAboutPaletteCount,
        this,
        &PaletterGUI::setPaletteCount
    );

    connect(
        myImgViewer,
        &ImageViewer::tellBossAboutPaletteFill,
        this,
        &PaletterGUI::drawPalette
    );

    drawPalette(myImgViewer->getPalette());

    paletteviewerlayout->addWidget(myPaletteViewer);
    mainLayout->addLayout(paletteviewerlayout);

    setWindowTitle(tr("PaletterGUImazing"));
    resize(screenSize.width(), screenSize.height());
}

// resive event override
// this should handle resizing the image
void
PaletterGUI::resizeEvent(QResizeEvent *event)
{
    // scale image with window
    myImgViewer->handleResizing();
}

void
PaletterGUI::setPaletteCount(int count)
{
    myImgViewer->setPaletteCount(count);
}

void
PaletterGUI::drawPalette(QList<QColor> *palette)
{
    myPaletteViewer->drawPalette(palette);
}
