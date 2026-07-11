#include "sick_graphics_view.h"
#include "sick_inspector.h"
#include <QApplication>

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

    myIsBeingInspected = false;
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

    if (myIsBeingInspected)
    {
        QGraphicsItem* item = itemAt(event->position().toPoint());
        if (item && item->type() == QGraphicsPixmapItem::Type)
        {
            auto *pixmap_item = 
                    qgraphicsitem_cast<QGraphicsPixmapItem*>(item);

            // scene space, parent space, image space, fuck you space
            QImage image = pixmap_item->pixmap().toImage();
            QPointF scene_pos = mapToScene(event->position().toPoint());
            QPointF local = pixmap_item->mapFromScene(scene_pos);
            if (image.valid(local.toPoint())) 
            {
                QColor pixel_color = image.pixelColor(local.toPoint());
                SickInspector::displayColor(pixel_color, 
                                            event->globalPosition().toPoint());
            }
        }
    }

    QGraphicsView::mouseMoveEvent(event);
}


void
SickGraphicsView::keyPressEvent(QKeyEvent *event)
{
    // I doubt event can ever be nullptr but hey :idk:
    if (!event || event->key() != Qt::Key_I)
        return;
    myIsBeingInspected = !myIsBeingInspected;
    if (!myIsBeingInspected)
    {
        SickInspector::hideColor();
        QApplication::restoreOverrideCursor();
    }
    else
        QApplication::setOverrideCursor(Qt::CrossCursor);
}
