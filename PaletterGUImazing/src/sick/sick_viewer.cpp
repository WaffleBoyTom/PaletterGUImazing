#include "sick_viewer.h"

#include <QMessageBox>
#include <QtWidgets>

// TODO: scaling factor hardcoded to 1/2 right now
// might wanna change that innit
static const int theImageScaleFactor = 2;

SickViewer::SickViewer(QWidget *parent)
{
    // keep in touch with your parent
    myCreator = parent;
    initialize();
    myProcessorButton = new QPushButton(tr("Process Image"), this);
    connect(
        myProcessorButton,
        &QPushButton::clicked,
        this,
        &ImageViewer::processImage
    );
    myModeDropdown->addMenuItem(tr("Median Cut"));
    myModeDropdown->addMenuItem(tr("K-Means"));
    // populate layout
    layout();
}

void
SickViewer::openNautilus()
{
    // TODO: I wonder if /home resolves correctly on windows >?
    // not that I want to support that awful os but
    // you know...

    const QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Palettize this geezer"),    /* title of fileDialog */
        QDir::homePath(),               /* where to start the search */
        tr("Image Files (*.png *.jpg)") /* file filter */
    );

    // User closed the dialog, so don't error out
    if (fileName.isEmpty())
        return;

    // print selected file to console
    qDebug() << fileName;

    if (!loadImage(&fileName))
    {
        qDebug() << "Failed to load image";
        QMessageBox::information(
            this,
            QGuiApplication::applicationDisplayName(), /* title */
            tr("Failed to load image: %1")
                .arg(QDir::toNativeSeparators(fileName)) /* message text */
        );
    }
}

bool
SickViewer::loadImage(const QString *filename)
{
    myLineEdit->setText(*filename);

    if (!myLoadedImage.load(*filename))
        return false;

    const QPixmap scaled = resizeImage(&myLoadedImage);

    // loaded image successfully
    myImageHolder->setPixmap(scaled);

    return true;
}

QImage
SickViewer::getImage()
{
    // we want to load in the original image
    // not the resized one
    return myLoadedImage.toImage();
}

void
SickViewer::askForPalette()
{
    emit askBossForPalette();
}

QPixmap
SickViewer::resizeImage(QPixmap *imagedisplay)
{
    // by default images are pretty big
    // unlike other things...
    // this probably should scale based on
    // main window size
    // dividing by 2 for now, idk

    return imagedisplay->scaled(
        myCreator->height() / theImageScaleFactor, /* width */
        myCreator->width() / theImageScaleFactor,  /* height */
        Qt::KeepAspectRatio                        /* ar */
    );
}

void
SickViewer::handleResizing()
{
    if (myLoadedImage.isNull())
        return;

    const QPixmap scaled = resizeImage(&myLoadedImage);

    myImageHolder->setPixmap(scaled);
}

QList<QColor> *
SickViewer::getPalette()
{
    return &myColorPalette;
}

void
SickViewer::layout()
{
    // populate layout
    myLayout->addWidget(myLineEdit);
    myLayout->addWidget(myNautilusButton);
    myLayout->addWidget(myProcessorButton);
    myLayout->addWidget(myModeDropdown);
    myLayout->addWidget(myImageHolder);
}

void
SickViewer::initialize()
{
    // main layout
    myLayout = new QVBoxLayout(this);

    // my boy Ethan so good lookin'
    myLineEdit = new QLineEdit(tr("Ethan so sexy"), this);

    // nautilus button
    myNautilusButton = new QPushButton(tr("Open Image"), this);

    connect(
        myNautilusButton,
        &QPushButton::clicked,
        this,
        &ImageViewer::openNautilus
    );

    // image holder
    myImageHolder = new QLabel(tr(""), this);

    myImageHolder->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    myModeDropdown = new SickDropDown(this, tr("Mode"));

    setPaletteCount(6);

    // I don't think I should have to make a copy here >?
    QList<QColor> default_palette(50, QColor(0, 0, 0));
    myColorPalette = default_palette;
}
