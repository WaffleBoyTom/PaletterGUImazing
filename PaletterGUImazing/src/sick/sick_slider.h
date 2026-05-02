#ifndef SICK_SLIDER_H
#define SICK_SLIDER_H

#include <QtWidgets>

/*

A Sick Fuckin' Slider which
combines a slider and a line edit
For Sick Duders and Dudettes only
*/

class SickSlider : public QWidget
{
    Q_OBJECT

public:
    explicit SickSlider(QWidget *parent);
    int getValue();

public slots:
    void onSliderValueChanged();

private slots:
    void onLineEditValueChanged();

signals:
    void paletteCountChangedSignal();

protected:
    void setSliderProperties();
    void wheelEvent(QWheelEvent *event);

private:
    QHBoxLayout *myLayout;
    QSlider *mySlider;
    QLineEdit *mySliderValueDisplay;
};

#endif  // SICK_SLIDER_H
