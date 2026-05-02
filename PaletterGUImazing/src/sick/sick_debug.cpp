#include "sick_debug.h"

void
sickDebugGeometry(const QWidget *w, const std::string &name)
{
    if (!name.empty())
        qDebug() << name;

    // Geometry relative to the parent, excluding window frame. The geometry
    // is just a rectangle consisting of the widget postion and size (width +
    // height).
    qDebug() << "geometry = " << w->geometry();

    // Geometry relative to the parent, including the window frame.
    qDebug() << "frameGeometry = " << w->frameGeometry();

    // geometry aliases: QWidget::rect()
    // size aliases: QWidget::size(), {QWidget::width(), QWidget::height()}.
    // position aliases: QWidget::pos(), {QWidget::x(), QWidget::y()}.
}

void
sickDebugSizing(const QWidget *w, const std::string &name)
{
    if (!name.empty())
        qDebug() << name;

    // Recommended size
    qDebug() << "size hint (preferred size) = " << w->sizeHint();

    // Size policy
    qDebug() << "size policy = " << w->sizePolicy();

    // The widget can't be resized any smaller than this
    qDebug() << "minimum size = " << w->minimumSize();

    // The widget can't be resized any larger than this
    qDebug() << "maximum size = " << w->maximumSize();

    // How much the widget size increases on resizing the window.
    qDebug() << "size increment = " << w->sizeIncrement();

    // minimum aliases: QWidget::minimumWidth(), QWidget::minimumHeight()
    // maximum aliaes: QWidget::maximumWidth(), QWidget::maximumHeight()
}

void
sickDebugLayout(const QWidget *w, const std::string &name)
{
    if (!name.empty())
        qDebug() << name;

    // Margins around the contents of the widget
    qDebug() << "content margins = " << w->contentsMargins();

    // Spacing between widgets - QWidget::spacing()
}
