#include "imageviewer.h"

#include <QMessageBox>
#include <QtWidgets>

#include "image_processor.h"

// TODO: scaling factor hardcoded to 1/2 right now
// might wanna change that innit
static const int theImageScaleFactor = 2;

// #define TEST_GPU

ImageViewer::ImageViewer(QWidget *parent, bool paletteSource = true)
{
    // keep in touch with your parent
    myCreator = parent;

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

    // processor button
    // temp solution to allow for a different button
    // between the two image viewers
    // FIXME : we want a base class for these two viewers
    // with overrides so we don't do this nasty if stuff ?
    // FIXME : stuff this in a horizontal layout
    auto dropdowns = new QHBoxLayout(this);

    myModeDropdown = new SickDropDown(this, tr("Mode"));
    if (paletteSource)
    {
        myProcessorButton = new QPushButton(tr("Process Image"), this);

        connect(
            myProcessorButton,
            &QPushButton::clicked,
            this,
            &ImageViewer::processImage
        );
        myModeDropdown->setMenuItem(tr("Median Cut"));
        myModeDropdown->setMenuItem(tr("K-Means"));
    }
    else
    {
        myProcessorButton = new QPushButton(tr("Apply Palette to Image"), this);

        connect(
            myProcessorButton,
            &QPushButton::clicked,
            this,
            &ImageViewer::askForPalette
        );
        myModeDropdown->setMenuItem(tr("Distance"));
        myModeDropdown->setMenuItem(tr("Luminance"));
        myModeDropdown->setMenuItem(tr("Hue"));
        myModeDropdown->setMenuItem(tr("Saturation"));
    }

    myDeviceDropdown = new SickDropDown(this, tr("Device"));
    myDeviceDropdown->setMenuItem(tr("CPU"));

#ifdef Q_OS_MACOS
    myDeviceDropdown->setMenuItem(tr("Metal"));
#else
    myDeviceDropdown->setMenuItem(tr("CUDA"));
#endif
    // init ImageProcessor
    myImageProcessor = ImageProcessor();

    setPaletteCount(6);

    // I don't think I should have to make a copy here >?
    QList<QColor> default_palette(50, QColor(0, 0, 0));
    myColorPalette = default_palette;

    // populate layout
    myLayout->addWidget(myLineEdit);
    myLayout->addWidget(myNautilusButton);
    myLayout->addWidget(myProcessorButton);
    dropdowns->addWidget(myModeDropdown);
    dropdowns->addWidget(myDeviceDropdown);
    myLayout->addLayout(dropdowns);
    myLayout->addWidget(myImageHolder);
}

void
ImageViewer::openNautilus()
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
    emit tellBossToLog("Loaded Image :\n");
    emit tellBossToLog(fileName + '\n');

    if (!loadImage(&fileName))
    {
        emit tellBossToLog("Failed to load image");
        QMessageBox::information(
            this,
            QGuiApplication::applicationDisplayName(), /* title */
            tr("Failed to load image: %1")
                .arg(QDir::toNativeSeparators(fileName)) /* message text */
        );
    }
}

bool
ImageViewer::loadImage(const QString *filename)
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
ImageViewer::getImage()
{
    // we want to load in the original image
    // not the resized one
    return myLoadedImage.toImage();
}

void
ImageViewer::processImage()
{
    if (myLoadedImage.isNull())
        return;

    QImage image = getImage();
    // myImageProcessor.fillColorPalette(image, myColorPalette, myPaletteCount);
    // myColorPalette = Quantizer(myPaletteCount).generatePalette(image);

    // FIXME: This is a horrible hack to keep the palette at a size of 50
    // and only copy what was asked for..
    QVector<QColor> new_palette = myImageProcessor.createColorPalette(
        image, myPaletteCount, Quantizer::Method::MedianCut
    );
    for (int i = 0; i < myPaletteCount; ++i)
    {
        myColorPalette[i] = new_palette[i];
    }
    emit tellBossToLog("filled color palette\n");

    // override orig with processed to make sure
    // result stays the same when we resize
    myLoadedImage = QPixmap::fromImage(image);

    myImageHolder->setPixmap(resizeImage(&myLoadedImage));
    emit tellBossAboutPaletteFill(&myColorPalette);
}

void
ImageViewer::askForPalette()
{
    emit askBossForPalette();
}

void
ImageViewer::applyPalette(QList<QColor> *palette)
{
    QImage image = getImage();
    auto colorspace = Remapper::Colorspace(myModeDropdown->item());

    auto device = PaletteProcessorDevice(myDeviceDropdown->item());

    // FIXME : logging here does not work ?

    emit tellBossToLog("Applying color palette :\n");

    QString dev_str(tr("Using: "));
    dev_str.append(getDeviceStr(device));
    emit tellBossToLog(dev_str);

    myImageProcessor.applyColorPalette(image, palette, device, colorspace);

    emit tellBossToLog("Done applying color palette: \n");

    myLoadedImage = QPixmap::fromImage(image);
    myImageHolder->setPixmap(resizeImage(&myLoadedImage));
}

QPixmap
ImageViewer::resizeImage(QPixmap *imagedisplay)
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
ImageViewer::handleResizing()
{
    if (myLoadedImage.isNull())
        return;

    const QPixmap scaled = resizeImage(&myLoadedImage);

    myImageHolder->setPixmap(scaled);
}

void
ImageViewer::setPaletteCount(const int count)
{
    myPaletteCount = count;
}

QList<QColor> *
ImageViewer::getPalette()
{
    return &myColorPalette;
}
