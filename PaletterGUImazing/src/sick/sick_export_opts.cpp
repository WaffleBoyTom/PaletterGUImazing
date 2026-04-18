#include "sick_export_opts.h"
#include <QTimer>


static constexpr int 
theNumberOfFormats = static_cast<int>(SickExportOpts::ExportFormat::INVALID);

static const char 
*theExportFormatMap[theNumberOfFormats] = {
    "HEX",
    "RGB (Float)",
    "RGB (Int)",
    "HSV (Float)",
    "HSV (Int)",    
    "HSL (Float)",
    "HSL (Int)",
    "CMYK (Float)",
    "CMYK (Int)"
};


SickExportOpts::SickExportOpts(QWidget *parent) 
: QWidget(parent)
{
    // setMinimumSize(screen()->geometry().width() / 2, 150);
    myLayout = new QVBoxLayout();
    myLayout->setAlignment(Qt::AlignTop);
    myExportButton = new QPushButton(tr("Export Palette"));
    connect(
        myExportButton,
        &QPushButton::clicked,
        this,
        &SickExportOpts::exportPalette
    );
    
    myExportPath = new SickFileLineEdit(
        this, 
        tr("Export Path"),
        SickFileLineEdit::Mode::WRITE
    );

    connect(
        myExportPath,
        &SickFileLineEdit::tellBossAboutFileLoaded,
        this,
        &SickExportOpts::logPathSet
    );
    
    myExportFormat = new SickDropDown(nullptr, tr("Format"));
    for (int i = 0; i < theNumberOfFormats; ++i)
        myExportFormat->addMenuItem(tr(theExportFormatMap[i]));
    
    myLayout->addWidget(myExportPath);
    myLayout->addWidget(myExportFormat);
    myLayout->addWidget(myExportButton);

    setLayout(myLayout);
}

void 
SickExportOpts::exportPalette()
{
    emit tellBossAboutExportOpts(
        myExportPath->text(),
        ExportFormat(myExportFormat->item())
    );
}

QString
SickExportOpts::getFormatToken(SickExportOpts::ExportFormat fmt)
{
    if (fmt == SickExportOpts::ExportFormat::INVALID)
        return QString("INVALID");
    const int index = static_cast<int>(fmt);
    if (index > theNumberOfFormats)
        return QString("INVALID");
    return QString(theExportFormatMap[index]);
    
}


void
SickExportOpts::logPathSet(const QString &filename)
{
    const QString native_path = QDir::toNativeSeparators(filename);
    QString message = QString("Set Export Path: %1").arg(native_path);
    SickLogger::log(message, SickLogSeverity::SEL); 
}
