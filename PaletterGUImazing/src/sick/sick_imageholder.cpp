#include "sick_imageholder.h"

#include <QString>
#include <QtWidgets>

/*
Subclass of QLabel
so you can resize an image
*/

SickImageHolder::SickImageHolder(QWidget *parent, 
                                 QString label) 
: QLabel(label, parent)
{
}

void 
SickImageHolder::mousePressEvent(QMouseEvent *event) 
{
    if (event->button() == Qt::LeftButton) 
        myStartDragPos = event->pos();
    QLabel::mousePressEvent(event);
}

void 
SickImageHolder::mouseMoveEvent(QMouseEvent *event) 
{
    if (!(event->buttons() & Qt::LeftButton)) 
        return;
    
    if ((event->pos() - myStartDragPos).manhattanLength() 
         < QApplication::startDragDistance())
        return;

    // kinda fun but pointless
    // move(pos() + event->pos() - myStartDragPos);
    
    const QPoint pos = event->pos();
    emit tellBossToResize(pos.x(), pos.y());
    
    event->accept();
}

void 
SickImageHolder::mouseReleaseEvent(QMouseEvent *event) 
{
    QLabel::mouseReleaseEvent(event);
}
