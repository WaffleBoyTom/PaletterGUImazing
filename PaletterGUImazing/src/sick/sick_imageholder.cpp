#include "sick_imageholder.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QString>
#include <QtWidgets>

/*
Subclass of QLabel
so you can resize an image
*/

SickImageHolder::SickImageHolder(QWidget *parent, QString label)
    : QLabel(label, parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(
        this,
        &QWidget::customContextMenuRequested,
        this,
        &SickImageHolder::showContextMenu
    );
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

    if ((event->pos() - myStartDragPos).manhattanLength() <
        QApplication::startDragDistance())
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

void
SickImageHolder::showContextMenu(const QPoint &pos)
{
    QMenu contextMenu(tr("Context menu"), this);
    QAction *act = new QAction("View Image Properties");
    contextMenu.addAction(act);
    connect(
        act, &QAction::triggered, this, &SickImageHolder::displayProperties
    );
    contextMenu.exec(mapToGlobal(pos));
}

void
SickImageHolder::displayProperties()
{
    // FIXME: This isn't super useful, we probably want to ask parent about
    // some more useful info ?
    auto data = QString("Width: %1\nHeight: %2")
                    .arg(pixmap().width())
                    .arg(pixmap().height());

    QMessageBox::information(this, "Image Properties", data);
}
