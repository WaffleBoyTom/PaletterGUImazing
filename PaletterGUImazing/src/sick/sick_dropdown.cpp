#include "sick_dropdown.h"

#include <QString>
#include <QtWidgets>

/*

Custom ComboBox that also has a label because
QT is very atomic
houdini has spoiled me for too long i guess >?

*/

SickDropDown::SickDropDown(QWidget *parent, QString label) : QWidget(parent)
{
    myLayout = new QHBoxLayout(this);
    myLabel = new QLabel(label, this);
    myMenu = new QComboBox(this);
    myLayout->addWidget(myLabel);
    myLayout->addWidget(myMenu);
}

void
SickDropDown::addMenuItem(QString item)
{
    myMenu->addItem(item);
}

void
SickDropDown::setMenuItem(int idx)
{
    myMenu->setCurrentIndex(idx);
}

int
SickDropDown::item() const
{
    return myMenu->currentIndex();
}
