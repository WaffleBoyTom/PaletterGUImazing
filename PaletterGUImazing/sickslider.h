#ifndef SICKSLIDER_H
#define SICKSLIDER_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
class QAction;
class QWidget;
class QSlider;
class QLineEdit;
class QHBoxLayout;
QT_END_NAMESPACE

class SickSlider : public QWidget
{
    Q_OBJECT

public:
    explicit SickSlider(QWidget *parent);
    int getValue();

private slots:

    void onSliderValueChanged();
    void onLineEditValueChanged();

protected:

    void setSliderProperties();


private:

    QWidget *myCreator; 
    QHBoxLayout *myLayout;
    QSlider *mySlider;
    QLineEdit *mySliderValueDisplay;

};

#endif
