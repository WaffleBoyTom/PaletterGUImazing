#ifndef SICKSLIDER_H
#define SICKSLIDER_H

#include <QWidget>
#include <QSlider>

QT_BEGIN_NAMESPACE
class QAction;
class QWidget;
class QSlider;
QT_END_NAMESPACE

class SickSlider : public QSlider
{
    Q_OBJECT

public:
    explicit SickSlider(QWidget *parent);

private slots:

protected:

    virtual void sliderChange(SliderChange change);

private:

    QWidget *myCreator; 

};

#endif
