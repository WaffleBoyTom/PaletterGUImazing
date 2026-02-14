#include "palette_viewer.h"

#include <QtWidgets>

#include "palette_row.h"
#include "sick_debug.h"
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

    myExportButton = new QPushButton(tr("Export Palette"), this);
    connect(
        myExportButton,
        &QPushButton::clicked,
        this,
        &PaletteViewer::exportPalette
    );

    myLayout->addWidget(myPaletteRow);
    myLayout->addWidget(mySlider);
    myLayout->addWidget(myExportButton);
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
    
    //TODO: make sure we always export to json
    // cuz user might type something like ballllls.jpg
    // this is not enough because user could type
    // fuck it up royally but hey, whatever 
    if (!native_path.endsWith(".json"))
    {
        SickLogger::log(
            QString("File name should end in .json.\nBy default we append the .json extension for you !"),
            SickLogSeverity::ERROR
        );
        return;
    }
    
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

    
    QJsonDocument doc(json);
    
    QByteArray json_data = doc.toJson(QJsonDocument::Indented);

    QFile out_file(native_path);

    // TODO: I don't trust these flags, idk what they do
    // look into these sus flags
    if (!out_file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) 
    {
        SickLogger::log(
            QString("Something went wrong with that file !!!!"), 
            SickLogSeverity::ERROR
        );
        return;
    }

    out_file.write(doc.toJson()); 
    out_file.close();

    SickLogger::log(QString(json_data));
    SickLogger::log(QString(native_path));
    
}
