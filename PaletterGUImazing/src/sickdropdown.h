#ifndef SICKDROPDOWN_H
#define SICKDROPDOWN_H

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
	// FIXME this seems like a shit name because I would expect
	// this method to take in an index....
	void setMenuItem(QString item);
    int item();
	
private:
    QWidget *myCreator;
    QHBoxLayout *myLayout;
    QComboBox *myMenu;
    QLabel *myLabel;
};

#endif
