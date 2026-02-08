#ifndef PALETTEVIEWER_H
#define PALETTEVIEWER_H

#include <QImage>
#include <QMainWindow>
#include <QWidget>

#include "paletterow.h"
#include "sick_slider.h"

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

public slots:
    void onPaletteCountChanged();
    void drawPalette(QList<QColor> *palette);

signals:
    void tellBossAboutPaletteCount(int count);

protected:
    void exportPalette();

private:
    QWidget *myCreator;
    QVBoxLayout *myLayout;
    PaletteRow *myPaletteRow;
    SickSlider *mySlider;
    QPushButton *myExportButton;
};

#endif
