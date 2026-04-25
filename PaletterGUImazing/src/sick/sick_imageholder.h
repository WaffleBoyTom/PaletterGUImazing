#ifndef SICK_IMAGEHOLDER_H
#define SICK_IMAGEHOLDER_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
class QLabel;
class QWidget;
class QPoint;
class QString;
class QMouseEvent;
QT_END_NAMESPACE

/*
Subclass of QLabel
so you can resize an image
*/

class SickImageHolder : public QLabel
{
    Q_OBJECT

public:
    explicit SickImageHolder(QWidget *parent, QString label);

signals:
    void tellBossToResize(int width, int height);

public slots:
    void showContextMenu(const QPoint &pos);
    void displayProperties();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:

    QPoint myStartDragPos;

};

#endif  // SICK_IMAGEHOLDER_H
