#ifndef PALETTER_WINDOW_H
#define PALETTER_WINDOW_H

#include <QWidget>

#include "cool/cool_logger.h"
#include "destination_pane.h"
#include "palette_viewer.h"
#include "sick/sick_export_opts.h"
#include "sick/sick_log_viewer.h"
#include "source_pane.h"

// FIXME: This should be a QMainWindow ?? :sadpanda:
class PaletterGUI : public QWidget
{
    Q_OBJECT

public:
    PaletterGUI();

public slots:
    void setPaletteDisplaySize(int size);
    void setPalette(QList<QColor> *palette);
    void applyPaletteToSecondViewer();
    void logMeHard(const QString msg, const CoolLogSeverity sev);
    void exportPalette(const QString &path, SickExportOpts::ExportFormat fmt);

private:
    QLabel *paletterLabel;
    SourcePane *mySrcImgViewer;
    PaletteViewer *myPaletteViewer;
    DestinationPane *myConvertImgViewer;
    SickLogViewer *myLogViewer;
    SickExportOpts *myExporter;
    [[maybe_unused]] QMenuBar *myMenuBar;
};

#endif  // PALETTER_WINDOW_H
