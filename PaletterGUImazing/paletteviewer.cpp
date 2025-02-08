#include "paletteviewer.h"

#include <QtWidgets>

PaletteViewer::PaletteViewer(QWidget *parent)
{
    // keep in touch with your parent
    myCreator = parent;

    // main layout
    myLayout = new QVBoxLayout(this); 
    mySlider = new QSlider(
        Qt::Horizontal,
        this
    );
    
    // the base QT Slider sucks mega fat donkey schlong
    // and we should subclass it to have something usable
    // basic shit like displaying what value you're on
    // ...
    // quite surprised that isn't built in ???
    // slider properties
    mySlider->setTickInterval(6);
    mySlider->setTickPosition(QSlider::TicksBothSides);
    mySlider->setMinimum(2);
    // what makes sense here -- if anything ?
    mySlider->setMaximum(50);
    // should a default like this be defined elsewhere >?
    mySlider->setValue(6);
    mySlider->setSingleStep(1);
    
    myLayout->addWidget(mySlider);
}

