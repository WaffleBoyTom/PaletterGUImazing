#include "palette_viewer.h"

#include <QtWidgets>

#include "palette_row.h"
#include "sick_debug.h"
#include "sick_fileio.h"
#include "sick_slider.h"
#include "sick_logger.h"

PaletteViewer::PaletteViewer(QWidget *parent) : QWidget(parent)
{
    // main layout
    myLayout = new QVBoxLayout(this);

    myPaletteRow = new PaletteRow(this);

    mySlider = new SickSlider(this);
    connect(
        mySlider,
        &SickSlider::paletteCountChangedSignal,
        this,
        &PaletteViewer::onPaletteSizeChanged
    );

    // myExportButton = new QPushButton(tr("Export Palette"), this);
    // connect(
    //     myExportButton,
    //     &QPushButton::clicked,
    //     this,
    //     &PaletteViewer::exportPalette
    // );
    

    myLayout->addWidget(myPaletteRow);
    myLayout->addWidget(mySlider);
    // myLayout->addWidget(myExportButton);
}

void
PaletteViewer::onPaletteSizeChanged()
{
    const int size = mySlider->getValue();
    myPaletteRow->onPaletteDisplaySizeChanged(size);

    emit tellBossAboutPaletteDisplaySize(size);
}

void
PaletteViewer::onPaletteChanged(QList<QColor> *palette)
{
    myPaletteRow->onPaletteChanged(palette);
}

void
PaletteViewer::exportPalette()
{
    SickLogger::log("Exporting Palette !");
    QFileDialog dialog(this);
    
    dialog.setWindowTitle(tr("Exporto Palettum !"));
    dialog.setDirectory(QDir::homePath());
    dialog.setAcceptMode(QFileDialog::AcceptMode::AcceptSave);
    dialog.setDefaultSuffix(QString(".json"));

    if (dialog.exec() != QDialog::Accepted)
    {
        SickLogger::log("Not doing nothing... Change of heart ?");
        return;
    }
    
    QString file_path = dialog.selectedFiles().first();
    
    const QString native_path = QDir::toNativeSeparators(file_path);
        
    QJsonObject json;
    bool can_serialize = myPaletteRow->serialize(json);
    if (!can_serialize)
    {
        // shit went south big time
        SickLogger::log(
            QString("Either you never generated a palette or shit went south big time..."),
            SickLogSeverity::ERROR
        );
        return;
    }

    SickJsonIO io(&json, &native_path);
    QString log;
    bool success = io.write(log);
    SickLogger::log(
        log,
        success ? SickLogSeverity::MSG 
                : SickLogSeverity::ERROR
    );
    
}
