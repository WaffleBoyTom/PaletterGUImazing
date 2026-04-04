#include "sick_export_opts.h"
#include <QTimer>


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
    myExportPath = new SickFileLineEdit(this, tr("Export Path"));

    myExportFormat = new SickDropDown(nullptr, tr("Format"));
    myExportFormat->addMenuItem(tr("RGB"));
    myExportFormat->addMenuItem(tr("HEX"));
    
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
