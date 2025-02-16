#ifndef PALETTEVIEWER_H
#define PALETTEVIEWER_H

#include <QMainWindow>
#include <QImage>
#include <QWidget>
#include "sickslider.h"

QT_BEGIN_NAMESPACE
class QAction;
class QWidget;
class QVBoxLayout;
class QPushButton;
QT_END_NAMESPACE

class PaletteViewer : public QWidget
{
    Q_OBJECT

public:
    explicit PaletteViewer(QWidget *parent);

private slots:
    
protected:
    void paintEvent(QPaintEvent *event) override;

private:

    QVBoxLayout *myLayout;
    // pointer to paletter window
    QWidget *myCreator;
    SickSlider *mySlider;
    QPushButton *myExportButton;
};

#endif
