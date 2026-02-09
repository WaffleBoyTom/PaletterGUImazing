#include "palette_viewer.h"

#include <QtWidgets>

#include "sick_debug.h"
#include "sick_slider.h"

PaletteViewer::PaletteViewer(QWidget *parent) : QWidget(parent)
{
    // main layout
    myLayout = new QVBoxLayout(this);

    myPaletteRow = new PaletteRow(this);
    mySlider = new SickSlider(this);
    myExportButton = new QPushButton(tr("Export Palette"), this);

    connect(
        mySlider,
        &SickSlider::paletteCountChangedSignal,
        this,
        &PaletteViewer::onPaletteSizeChanged
    );

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
    qDebug() << "balllls";
}
