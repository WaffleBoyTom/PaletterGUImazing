#include "sickslider.h"

#include <QtWidgets>
#include <QString>

/*
Custom slider because base qt slider is more cheeks
than maurice...
houdini has spoiled me for too long i guess >?

*/

SickSlider::SickSlider(QWidget *parent)
{
    // keep in touch with your parent
    myCreator = parent;
    myLayout = new QHBoxLayout(this);
    
    mySlider = new QSlider(this);
    setSliderProperties();
    
    mySliderValueDisplay = new QLineEdit("6", this);
    // adds callback to slider
    connect(
        mySlider,
        &QSlider::valueChanged,
        this,
        &SickSlider::onSliderValueChanged
    );
    // adds callback to Line Edit
    connect(
        mySliderValueDisplay,
        &QLineEdit::textChanged,
        this,
        &SickSlider::onLineEditValueChanged  
    );

    myLayout->addWidget(mySlider);
    myLayout->addWidget(mySliderValueDisplay);
    
}

void
SickSlider::setSliderProperties()
{
    // bunch of magic numbers here
    
    mySlider->setOrientation(Qt::Horizontal);
    mySlider->setTickInterval(2);
    mySlider->setTickPosition(QSlider::TicksBothSides);
    mySlider->setMinimum(2);
    mySlider->setMaximum(50);
    mySlider->setValue(6);
    mySlider->setSingleStep(1);
}

void
SickSlider::onSliderValueChanged()
{
    // update Label when user drags slider
    mySliderValueDisplay->setText(
        QString::number(mySlider->value())
    );
    // forces paintEvent to get called on PaletteViewer
    myCreator->repaint();
}

void
SickSlider::onLineEditValueChanged()
{
    // update slider
    mySlider->setValue(
        mySliderValueDisplay->text().toInt()
    );
    // forces paintEvent to get called on PaletteViewer
    myCreator->repaint();
}

int
SickSlider::getValue()
{
    return mySlider->value();
}
