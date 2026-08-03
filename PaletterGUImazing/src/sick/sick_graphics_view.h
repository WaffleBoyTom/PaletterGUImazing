#ifndef SICK_GRAPHICS_VIEW_H
#define SICK_GRAPHICS_VIEW_H

#include <QtWidgets>

class SickGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    SickGraphicsView(QGraphicsScene *scene, QWidget *parent);

    void frameItem(QGraphicsItem *item);
    bool
    isBeingInspected()
    {
        return myIsBeingInspected;
    }

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QTransform myInitialTransform;
    QPointF myInitialDragPosition;
    bool myIsBeingInspected;
};

#endif  // SICK_GRAPHICS_VIEW_H
