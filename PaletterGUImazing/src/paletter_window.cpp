#include "paletter_window.h"

#include <QPushButton>
#include <QThread>
#include <QtWidgets>

#include "image_viewer.h"
#include "sick_logger.h"
#include "palette_viewer.h"
#include "sick_log_viewer.h"

static const int theMinSizeScaleFactor = 4;

PaletterGUI::PaletterGUI() : paletterLabel(new QLabel(this))
{
    const QRect screenGeometry = screen()->geometry();
    const QSize screenSize = screen()->size();

    // how small you can resize the window
    this->setMinimumSize(
        screenGeometry.width() / theMinSizeScaleFactor,
        screenGeometry.height() / theMinSizeScaleFactor
    );

    // myMenuBar = new QMenuBar(this);
    // QMenu *menu = new QMenu("File");
    // menu->addMenu("Hell Yeah !");
    // myMenuBar->addMenu(menu);
    
    // Outermost layout of the app.
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *viewersLayout = new QHBoxLayout();

    // add the image viewer here
    myImgViewer = new ImageViewer(this, true /*paletteSource*/);
    viewersLayout->addWidget(myImgViewer);

    // add the other image viewer here
    // this is the one where you can apply the palette to an image
    myConvertImgViewer = new ImageViewer(this, false /* paletteSource */);
    viewersLayout->addWidget(myConvertImgViewer);

    mainLayout->addLayout(viewersLayout);

    // add something random to assert my dominance
    // QVBoxLayout *buttonsLayout = new QVBoxLayout();
    // newLineEdit = new QLineEdit(tr("I'm a 10x programmer"), this);
    // buttonsLayout->addWidget(newLineEdit);
    // mainLayout->addLayout(buttonsLayout);

    // add the palette viewer here
    QVBoxLayout *paletteviewerlayout = new QVBoxLayout();
    myPaletteViewer = new PaletteViewer(this);

    // when the slider is changed, set the palette count
    // on the image processor
    connect(
        myPaletteViewer,
        &PaletteViewer::tellBossAboutPaletteDisplaySize,
        this,
        &PaletterGUI::setPaletteDisplaySize
    );

    connect(
        myImgViewer,
        &ImageViewer::tellBossAboutPaletteFill,
        this,
        &PaletterGUI::setPalette
    );

    connect(
        myConvertImgViewer,
        &ImageViewer::askBossForPalette,
        this,
        &PaletterGUI::applyPaletteToSecondViewer
    );

    setPalette(myImgViewer->palette());

    paletteviewerlayout->addWidget(myPaletteViewer);
    mainLayout->addLayout(paletteviewerlayout);

    QHBoxLayout *utils_layout = new QHBoxLayout();
    
    myLogViewer = new SickLogViewer(this);
    connect(
        SickLogger::getInstance(),
        &SickLogger::tellBossToLog,
        this,
        &PaletterGUI::logMeHard
    );

    utils_layout->addWidget(myLogViewer);

    myExporter = new SickExportOpts(nullptr);
    connect(
        myExporter,
        &SickExportOpts::tellBossAboutExportOpts,
        this,
        &PaletterGUI::exportPalette
    );

    utils_layout->addWidget(myExporter);

    mainLayout->addLayout(utils_layout);

    setWindowTitle(tr("PaletterGUImazing"));
    resize(screenSize.width(), screenSize.height());
}

// resive event override
// this should handle resizing the image
void
PaletterGUI::resizeEvent(QResizeEvent *event)
{
    // scale image with window
    myImgViewer->handleResizing();
    // we have myConvertImgViewer,
    // it probably shoud be resized here
}

void
PaletterGUI::setPaletteDisplaySize(int size)
{
    myImgViewer->setPaletteDisplaySize(size);
}

void
PaletterGUI::setPalette(QList<QColor> *palette)
{
    myPaletteViewer->onPaletteChanged(palette);
}

void
PaletterGUI::applyPaletteToSecondViewer()
{
    myConvertImgViewer->applyPalette(myImgViewer->palette());
}

void
PaletterGUI::logMeHard(const QString msg, const SickLogSeverity sev)
{
    myLogViewer->appendLine(msg, sev);
}

void
PaletterGUI::exportPalette(const QString &path, 
                           SickExportOpts::ExportFormat fmt)
{
    myPaletteViewer->exportPalette(path, fmt);
}
