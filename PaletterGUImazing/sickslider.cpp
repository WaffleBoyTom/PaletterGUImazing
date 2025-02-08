#include "sickslider.h"

#include <QtWidgets>

/*
Custom slider because base qt slider is more cheeks
than maurice...
houdini has spoiled me for too long i guess >?

*/

SickSlider::SickSlider(QWidget *parent) : QSlider(parent)
{
    // keep in touch with your parent
    myCreator = parent;
    
    setOrientation(Qt::Horizontal);
    setTickInterval(6);
    setTickPosition(QSlider::TicksBothSides);
    setMinimum(2);
    // what makes sense here -- if anything ?
    setMaximum(50);
    // should a default like this be defined elsewhere >?
    setValue(6);
    setSingleStep(1);
}

