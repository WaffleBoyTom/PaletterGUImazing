#ifndef PALETTE_VIEWER_H
#define PALETTE_VIEWER_H

#include <QImage>
#include <QMainWindow>
#include <QWidget>

#include "palette_row.h"
#include "sick_slider.h"
#include "sick_export_opts.h"

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
    void exportPalette(const QString &path, SickExportOpts::ExportFormat fmt);

public slots:
    void onPaletteSizeChanged();
    void onPaletteChanged(QList<QColor> *palette);

signals:
    void tellBossAboutPaletteDisplaySize(int count);

protected:

private:
    QVBoxLayout *myLayout;
    PaletteRow *myPaletteRow;
    SickSlider *mySlider;
    QPushButton *myExportButton;
};

#endif  // PALETTE_VIEWER_H
