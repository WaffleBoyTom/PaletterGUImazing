#include "sick_image_viewer.h"

SickImageViewer::SickImageViewer(QWidget *parent): QWidget(parent)
{
    myScene = new QGraphicsScene(this);
    myImageItem = nullptr;

    mySceneView = new SickGraphicsView(myScene, this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(mySceneView);
    setLayout(layout);
}

void
SickImageViewer::setImage(QImage image)
{
    const QPixmap pixmap = QPixmap::fromImage(image);

    // Remove the old item.
    if (myImageItem != nullptr)
    {
        myScene->removeItem(myImageItem);
        delete myImageItem;
    }

    myImageItem = myScene->addPixmap(pixmap);

    const QRectF rect = myImageItem->boundingRect();
    const float padX = rect.width() * 5.0;
    const float padY = rect.height() * 5.0;
    myScene->setSceneRect(rect.adjusted(-padX, -padY, padX, padY));
}

void
SickImageViewer::frameImage()
{
    if (myImageItem == nullptr)
        return;

    mySceneView->frameItem(myImageItem);
}
