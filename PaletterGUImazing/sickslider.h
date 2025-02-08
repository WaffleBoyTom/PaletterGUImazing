#ifndef SICKSLIDER_H
#define SICKSLIDER_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
class QAction;
class QWidget;
class QSlider;
class QLabel;
class QHBoxLayout;
QT_END_NAMESPACE

class SickSlider : public QWidget
{
    Q_OBJECT

public:
    explicit SickSlider(QWidget *parent);

private slots:

    void onSliderValueChanged();

protected:

    void setSliderProperties();


private:

    QWidget *myCreator; 
    QHBoxLayout *myLayout;
    QSlider *mySlider;
    QLabel *mySliderValueDisplay;

};

#endif
