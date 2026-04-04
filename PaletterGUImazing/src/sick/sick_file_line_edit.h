#ifndef SICK_FILELINEEDIT_H
#define SICK_FILELINEEDIT_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
class QAction;
class QWidget;
class QPushButton;
class QLineEdit;
class QHBoxLayout;
class QString;
QT_END_NAMESPACE

class SickFileLineEdit : public QWidget
{
    Q_OBJECT

public:
    explicit SickFileLineEdit(QWidget *parent, QString label);
    QString text();

private:
    void openNautilus();
    QHBoxLayout *myLayout;
    QPushButton *myFileChooser;
    QLineEdit   *myLineEdit;
};

#endif  // SICK_FILELINEEDIT_H
