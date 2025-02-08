#ifndef PALETTEVIEWER_H
#define PALETTEVIEWER_H

#include <QMainWindow>
#include <QImage>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QAction;
class QWidget;
class QSlider;
class QVBoxLayout;
QT_END_NAMESPACE

class PaletteViewer : public QWidget
{
    Q_OBJECT

public:
    explicit PaletteViewer(QWidget *parent);

private slots:
    

private:

    QVBoxLayout *myLayout;
    // pointer to paletter window
    QWidget *myCreator;
    QSlider *mySlider;
};

#endif
