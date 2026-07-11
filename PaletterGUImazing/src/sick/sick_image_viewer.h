#ifndef SICK_IMAGE_VIEWER_H
#define SICK_IMAGE_VIEWER_H

#include <QtWidgets>

#include "sick_graphics_view.h"

class SickImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit SickImageViewer(QWidget *parent);

    void setImage(QImage image);
    void frameImage();
    bool isBeingInspected() { return mySceneView->isBeingInspected(); }


private:
    QGraphicsScene *myScene;
    QGraphicsPixmapItem *myImageItem;

    SickGraphicsView *mySceneView;
};

#endif // SICK_IMAGE_VIEWER_H
