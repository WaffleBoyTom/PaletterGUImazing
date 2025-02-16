#include "paletteviewer.h"
#include "sickslider.h"
#include <QtWidgets>

PaletteViewer::PaletteViewer(QWidget *parent)
{
    // keep in touch with your parent
    myCreator = parent;

    // main layout
    myLayout = new QVBoxLayout(this); 
    mySlider = new SickSlider(this);
    myExportButton = new QPushButton(
        tr("Export Palette"), this);

    connect(
        myExportButton,
        &QPushButton::clicked,
        this,
        &PaletteViewer::exportPalette    
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
    
    QPainter painter(this);
    int start = 0;
    int num_boxes = mySlider->getValue();

    // the idea behind this padding is that
    // we get a bit of space between each rectangle
    int padding = 10;
    int size = screen()->size().width() / num_boxes;
    int height = this->size().height() / 4;

        
    for(int i = 0; i < num_boxes; ++i)
    {
        painter.fillRect(
            start + (size * i), // x
            height / 2, // y
            size - padding, // width
            height, // height
            QColor(88, i*3, i*2) // color
        );
    }

}

void
PaletteViewer::exportPalette()
{
    qDebug() << "balllls";
}

