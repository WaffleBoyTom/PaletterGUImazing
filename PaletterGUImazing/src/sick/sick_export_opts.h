#ifndef SICK_EXPORT_OPTS_H
#define SICK_EXPORT_OPTS_H

#include <QtWidgets>
#include "sick_dropdown.h"


class SickExportOpts : public QWidget
{
    Q_OBJECT

public:
    explicit SickExportOpts(QWidget *parent);
    enum class ExportFormat
    {
        RGB = 0,
        HEX
    };

signals:
    void tellBossAboutExportOpts(QString file_path, ExportFormat format);

private:
    
    void exportPalette();
    
    QVBoxLayout   *myLayout;
    QPushButton   *myExportButton;
    QLineEdit     *myExportPath;
    SickDropDown  *myExportFormat;
};

#endif  // SICK_EXPORT_OPTS_H
