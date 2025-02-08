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
        
    myLayout->addWidget(mySlider);
}

