#ifndef PALETTE_VIEWER_H
#define PALETTE_VIEWER_H

#include <QImage>
#include <QMainWindow>
#include <QWidget>

#include "palette_row.h"
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
    QVBoxLayout *myLayout;
    PaletteRow *myPaletteRow;
    SickSlider *mySlider;
    QPushButton *myExportButton;
};

#endif  // PALETTE_VIEWER_H
