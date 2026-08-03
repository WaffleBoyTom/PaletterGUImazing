#include "palette_viewer.h"

#include <QtWidgets>

#include "cool/cool_fileio.h"
#include "cool/cool_logger.h"
#include "palette_row.h"
#include "sick/sick_slider.h"

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
PaletteViewer::exportPalette(
    const QString &path, SickExportOpts::ExportFormat fmt
)
{
    CoolLogger::log("Exporting Palette !");
    // QFileDialog dialog(this);

    // dialog.setWindowTitle(tr("Exporto Palettum !"));
    // dialog.setDirectory(QDir::homePath());
    // dialog.setAcceptMode(QFileDialog::AcceptMode::AcceptSave);
    // dialog.setDefaultSuffix(QString(".json"));

    // if (dialog.exec() != QDialog::Accepted)
    // {
    //     CoolUtilLogger::log("Not doing nothing... Change of heart ?");
    //     return;
    // }

    // QString file_path = dialog.selectedFiles().first();

    // FIXME
    // SickExportOpts::ExportFormat does nawt right now ....

    const QString native_path = QDir::toNativeSeparators(path);

    QJsonObject json;
    bool can_serialize = myPaletteRow->serialize(json, fmt);
    if (!can_serialize)
    {
        // shit went south big time
        CoolLogger::log(
            QString(
                "Either you never generated a palette or shit went south big "
                "time..."
            ),
            CoolLogSeverity::ERROR
        );
        return;
    }

    CoolJsonIO io(&json, &native_path);
    QString log;
    bool success = io.write(log);
    CoolLogger::log(
        log, success ? CoolLogSeverity::MSG : CoolLogSeverity::ERROR
    );
}
