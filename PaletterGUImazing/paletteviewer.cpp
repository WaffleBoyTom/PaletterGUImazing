#include "paletteviewer.h"

#include <QtWidgets>

#include "sickslider.h"

PaletteViewer::PaletteViewer(QWidget *parent)
{
    // keep in touch with your parent
    myCreator = parent;

    // main layout
    myLayout = new QVBoxLayout(this);
    mySlider = new SickSlider(this);

    myExportButton = new QPushButton(tr("Export Palette"), this);

    connect(
        myExportButton,
        &QPushButton::clicked,
        this,
        &PaletteViewer::exportPalette
    );
    connect(
        mySlider,
        &SickSlider::paletteCountChangedSignal,
        this,
        &PaletteViewer::onPaletteCountChanged
    );

    myLayout->addWidget(mySlider);
    myLayout->addWidget(myExportButton);
}

void
PaletteViewer::paintEvent(QPaintEvent *event)
{
    // this is a start to paint the palette
    // maybe there's a cleverer way to do this
    // maybe we also don't draw over
    // the fuckin slider lmao
    // magic numbers galore...
    // and bad maths
    if (!myPalettePtr)
        return;

    QPainter painter(this);
    int start = 0;
    int num_boxes = mySlider->getValue();

    if (!myPalettePtr->size() == num_boxes)
        return;

    // the idea behind this padding is that
    // we get a bit of space between each rectangle
    int padding = 10;
    int size = screen()->size().width() / num_boxes;
    int height = this->size().height() / 4;

    // can we add text on mouse hover
    // that displays rgb and hex values >?

    for (int i = 0; i < num_boxes; ++i)
    {
        int index = qBound(0, i, 50);
        painter.fillRect(
            start + (size * i),  // x
            height / 2,          // y
            size - padding,      // width
            height,              // height
            myPalettePtr->at(index)
        );
    }
}

void
PaletteViewer::exportPalette()
{
    qDebug() << "balllls";
}

void
PaletteViewer::onPaletteCountChanged()
{
    int count = mySlider->getValue();
    emit tellBossAboutPaletteCount(count);
}

void
PaletteViewer::drawPalette(QList<QColor> *paletteptr)
{
    myPalettePtr = paletteptr;
    this->repaint();
}
