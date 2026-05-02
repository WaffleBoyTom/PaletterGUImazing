#ifndef SICK_DROPDOWN_H
#define SICK_DROPDOWN_H

#include <QtWidgets>

class SickDropDown : public QWidget
{
    Q_OBJECT

public:
    explicit SickDropDown(QWidget *parent, QString label);
    void addMenuItem(QString item);
    void setMenuItem(int idx);
    // returns the current selected menu item
    int item() const;

private:
    QHBoxLayout *myLayout;
    QComboBox *myMenu;
    QLabel *myLabel;
};

#endif  // SICK_DROPDOWN_H
