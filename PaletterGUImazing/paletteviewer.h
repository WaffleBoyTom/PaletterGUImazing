#ifndef PALETTEVIEWER_H
#define PALETTEVIEWER_H

#include <QImage>
#include <QMainWindow>
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

public slots:
    void onPaletteCountChanged();
    void drawPalette(QList<QColor> *palette);

private slots:

signals:
    void tellBossAboutPaletteCount(int count);

protected:
    void paintEvent(QPaintEvent *event) override;
    void exportPalette();

private:
    QVBoxLayout *myLayout;
    // pointer to paletter window
    QWidget *myCreator;
    SickSlider *mySlider;
    QPushButton *myExportButton;
    QList<QColor> *myPalettePtr;
};

#endif
