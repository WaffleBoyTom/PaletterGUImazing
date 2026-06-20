#include "sick_graphics_view.h"

SickGraphicsView::SickGraphicsView(QGraphicsScene *scene, QWidget *parent):
    QGraphicsView(scene, parent), myInitialTransform{}, myInitialDragPosition{}
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

    setDragMode(QGraphicsView::ScrollHandDrag);

    setTransformationAnchor(QGraphicsView::AnchorViewCenter);

    setMouseTracking(true);

    // TODO: set correct background color
    setBackgroundBrush(QBrush(QColor("#101010")));
}

void
SickGraphicsView::frameItem(QGraphicsItem *item)
{
    fitInView(item, Qt::KeepAspectRatio);
    scale(0.9, 0.9);
}

void
SickGraphicsView::mousePressEvent(QMouseEvent *event)
{
    myInitialTransform = transform();
    myInitialDragPosition = event->position();

    QGraphicsView::mousePressEvent(event);
}

void
SickGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::RightButton)
    {
        const QPointF position = event->position();
        const QPointF difference = position - myInitialDragPosition;

        float length = difference.manhattanLength();
        if (difference.x() < 0.0)
        {
            length *= -1.0f;
        }

        const float a = (width() + length) / width();
        const float view_scale = std::clamp(a, 0.01f, 100.0f);

        setTransform(myInitialTransform);
        scale(view_scale, view_scale);
    }

    QGraphicsView::mouseMoveEvent(event);
}
