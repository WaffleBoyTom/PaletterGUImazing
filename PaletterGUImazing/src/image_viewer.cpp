#include "image_viewer.h"

#include <QtWidgets/qpushbutton.h>

#include <QMessageBox>
#include <QtWidgets>

#include "baller_task.h"
#include "remapper.h"

// TODO: scaling factor hardcoded to 1/2 right now
// might wanna change that innit
static const int theImageScaleFactor = 2;

ImageViewer::ImageViewer(QWidget *parent, bool paletteSource = true)
    : QWidget(parent)
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

    // processor button
    // temp solution to allow for a different button
    // between the two image viewers
    // FIXME: we want a base class for these two viewers
    // with overrides so we don't do this nasty if stuff ?
    // FIXME: stuff this in a horizontal layout
    auto dropdowns = new QHBoxLayout();

    myModeDropdown = new SickDropDown(this, tr("Mode"));

    // TODO: inheritanceizationify this instead of using a flag lol
    if (paletteSource)
    {
        myProcessorButton = new QPushButton(tr("Process Image"), this);

        connect(
            myProcessorButton,
            &QPushButton::clicked,
            this,
            &ImageViewer::generatePalette
        );

        myProcessorButton->setEnabled(false);

        myModeDropdown->addMenuItem(tr("Median Cut"));
        myModeDropdown->addMenuItem(tr("K-Means"));
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

        myProcessorButton->setEnabled(false);

        myModeDropdown->addMenuItem(tr("Distance"));
        myModeDropdown->addMenuItem(tr("Luminance"));
        myModeDropdown->addMenuItem(tr("Hue"));
        myModeDropdown->addMenuItem(tr("Saturation"));
    }

    myDeviceDropdown = new SickDropDown(this, tr("Device"));
    myDeviceDropdown->addMenuItem(tr("CPU"));

#if defined(USE_METAL)
    myDeviceDropdown->setMenuItem(tr("Metal"));
#elif defined(USE_CUDA)
    myDeviceDropdown->setMenuItem(tr("CUDA"));
#endif

    setPaletteCount(6);

    myColorPalette = QList<QColor>(50, QColor(0, 0, 0));

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

    const QString file_name = QFileDialog::getOpenFileName(
        this,
        tr("Palettize this geezer"),    /* title of fileDialog */
        QDir::homePath(),               /* where to start the search */
        tr("Image Files (*.png *.jpg)") /* file filter */
    );

    // User closed the dialog, so don't error out
    if (file_name.isEmpty())
        return;

    // print selected file to console
    emit tellBossToLog(QString("Loaded image file: %1").arg(file_name));

    if (!loadImage(&file_name))
    {
        emit tellBossToLog(
            QString("Failed to load image file: %1").arg(file_name)
        );
        QMessageBox::information(
            this,
            QGuiApplication::applicationDisplayName(), /* title */
            tr("Failed to load image: %1")
                .arg(QDir::toNativeSeparators(file_name)) /* message text */
        );
    }
}

bool
ImageViewer::loadImage(const QString *filename)
{
    myLineEdit->setText(*filename);

    if (!myLoadedImage.load(*filename))
        return false;

    myProcessorButton->setEnabled(true);

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
ImageViewer::generatePalette()
{
    if (myLoadedImage.isNull())
        return;

    myProcessorButton->setEnabled(false);

    // TODO: the task should be hidden behind an ImageProcessor interface.
    // the ImageViewer should not create threads or tasks directly.
    QImage image = getImage();
    QuantizeTask *task =
        new QuantizeTask(image, myPaletteCount, Quantizer::Method::MedianCut);

    connect(
        task,
        &QuantizeTask::finished,
        this,
        &ImageViewer::onGeneratePaletteFinished
    );

    QThread *thread = new QThread();
    task->runOnThread(thread);
}

void
ImageViewer::onGeneratePaletteFinished(QList<QColor> palette)
{
    myColorPalette = palette.sliced(0, MAX_PALETTE_SIZE);
    emit tellBossToLog("Filled color palette");

    // override orig with processed to make sure
    // result stays the same when we resize
    QImage image = getImage();
    myLoadedImage = QPixmap::fromImage(image);

    myImageHolder->setPixmap(resizeImage(&myLoadedImage));
    emit tellBossAboutPaletteFill(&myColorPalette);

    myProcessorButton->setEnabled(true);
}

void
ImageViewer::applyPalette(QList<QColor> *palette)
{
    myProcessorButton->setEnabled(false);

    emit tellBossToLog("Applying color palette");

    QImage image = getImage();

    auto device = PaletteProcessorDevice(myDeviceDropdown->item());
    emit tellBossToLog(QString("Using: %1").arg(getDeviceStr(device)));

    auto method = Remapper::CompareMethod(myModeDropdown->item());

    // TODO: the task should be hidden behind an ImageProcessor interface.
    // the ImageViewer should not create threads or tasks directly.
    RemapTask *task = new RemapTask(image, device, method, *palette);

    connect(
        task, &RemapTask::finished, this, &ImageViewer::onApplyPaletteFinished
    );

    QThread *thread = new QThread();
    task->runOnThread(thread);
}

void
ImageViewer::onApplyPaletteFinished(QImage image)
{
    emit tellBossToLog("Done applying color palette");

    myLoadedImage = QPixmap::fromImage(image);
    myImageHolder->setPixmap(resizeImage(&myLoadedImage));

    myProcessorButton->setEnabled(true);
}

void
ImageViewer::askForPalette()
{
    emit askBossForPalette();
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
        parentWidget()->height() / theImageScaleFactor, /* width */
        parentWidget()->width() / theImageScaleFactor,  /* height */
        Qt::KeepAspectRatio                             /* ar */
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
