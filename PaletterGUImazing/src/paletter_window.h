#ifndef PALETTER_WINDOW_H
#define PALETTER_WINDOW_H

#include <QWidget>

#include "dst_image_viewer.h"
#include "palette_viewer.h"
#include "sick_export_opts.h"
#include "sick_log_viewer.h"
#include "src_image_viewer.h"

// FIXME: This should be a QMainWindow ?? :sadpanda:
class PaletterGUI : public QWidget
{
    Q_OBJECT

public:
    PaletterGUI();

protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void setPaletteDisplaySize(int size);
    void setPalette(QList<QColor> *palette);
    void applyPaletteToSecondViewer();
    void logMeHard(const QString msg, const SickLogSeverity sev);
    void exportPalette(const QString &path, SickExportOpts::ExportFormat fmt);

private:
    QLabel *paletterLabel;
    SrcImageViewer *mySrcImgViewer;
    PaletteViewer *myPaletteViewer;
    DstImageViewer *myConvertImgViewer;
    SickLogViewer *myLogViewer;
    SickExportOpts *myExporter;
    [[maybe_unused]] QMenuBar *myMenuBar;
};

#endif  // PALETTER_WINDOW_H
