#ifndef SICK_EXPORT_OPTS_H
#define SICK_EXPORT_OPTS_H

#include <QtWidgets>
#include "sick_dropdown.h"
#include "sick_file_line_edit.h"
#include "sick_logger.h"

/*

Widget that handles the export of the palette
has a SickFileLineEdit to get a file path to write to
has a SickDropDown to handle the export format
has a QPushButton to trigger the export

*/

class SickExportOpts : public QWidget
{
    Q_OBJECT

public:
    explicit SickExportOpts(QWidget *parent);
    enum class ExportFormat
    {
        HEX = 0,
        RGBF,
        RGBI,
        HSVF,
        HSVI,
        HSLF,
        HSLI,
        CMYKF,
        CMYKI,
        INVALID
    };

    /// return token associated with export format
    static QString getFormatToken(SickExportOpts::ExportFormat fmt);

signals:
    void tellBossAboutExportOpts(QString file_path, ExportFormat format);

public slots:
    void logPathSet(const QString &filename);

private:
    
    void exportPalette();
    
    QVBoxLayout          *myLayout;
    QPushButton          *myExportButton;
    SickFileLineEdit     *myExportPath;
    SickDropDown         *myExportFormat;
};

#endif  // SICK_EXPORT_OPTS_H
