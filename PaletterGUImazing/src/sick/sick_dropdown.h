#ifndef SICK_DROPDOWN_H
#define SICK_DROPDOWN_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
class QAction;
class QComboBox;
class QWidget;
class QSlider;
class QLineEdit;
class QHBoxLayout;
class QString;
QT_END_NAMESPACE

class SickDropDown : public QWidget
{
    Q_OBJECT

public:
    explicit SickDropDown(QWidget *parent, QString label);
    void addMenuItem(QString item);
    int item();

private:
    QWidget *myCreator;
    QHBoxLayout *myLayout;
    QComboBox *myMenu;
    QLabel *myLabel;
};

#endif  // SICK_DROPDOWN_H
