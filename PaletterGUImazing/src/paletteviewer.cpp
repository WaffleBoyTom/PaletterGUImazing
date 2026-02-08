#include "paletteviewer.h"

#include <QtWidgets>

#include "debug.h"
#include "sickslider.h"

PaletteViewer::PaletteViewer(QWidget *parent)
{
    // keep in touch with your parent
    myCreator = parent;

    // main layout
    myLayout = new QVBoxLayout(this);

    myPaletteRow = new PaletteRow(this);
    mySlider = new SickSlider(this);
    myExportButton = new QPushButton(tr("Export Palette"), this);
    
    connect(
        mySlider,
        &SickSlider::paletteCountChangedSignal,
        this,
        &PaletteViewer::onPaletteCountChanged
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
PaletteViewer::onPaletteCountChanged()
{
    const int count = mySlider->getValue();
    myPaletteRow->onPaletteCountChanged(count);

    emit tellBossAboutPaletteCount(count);
}

void
PaletteViewer::drawPalette(QList<QColor> *paletteptr)
{
    myPaletteRow->drawPalette(paletteptr);
}

void
PaletteViewer::exportPalette()
{
    qDebug() << "balllls";
}
