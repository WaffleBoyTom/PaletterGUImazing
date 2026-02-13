#include "image_viewer.h"

#include <QMessageBox>
#include <QtWidgets>

#include "baller_task.h"
#include "sick_logger.h"
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

    setPaletteDisplaySize(INIT_PALETTE_SIZE);
    myPalette = QList<QColor>();

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
    QFileDialog dialog(this);
    dialog.setWindowTitle(tr("Palettize this geezer"));
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFile);

    QList<QByteArray> image_formats = QImageReader::supportedImageFormats();
    QStringList format_filters;
    for (const QByteArray &image_format : image_formats)
        format_filters << QString("*.%1").arg(image_format);

    // Remove GIF filter.
    format_filters.removeAll("*.gif");

    QString all_name_filter =
        QString("All Images (%1)").arg(format_filters.join(" "));

    dialog.setNameFilter(all_name_filter);

    if (dialog.exec() == QDialog::Accepted)
    {
        QString file_path = dialog.selectedFiles().first();
        if (loadImage(file_path))
        {
            const QString native_path = QDir::toNativeSeparators(file_path);
            QString message = QString("Loaded image file: %1").arg(native_path);
            SickLogger::log(message);
        }
        else
        {
            QString message = "Failed to load image file";
            SickLogger::log(message);
            QMessageBox::information(
                this, QGuiApplication::applicationDisplayName(), message
            );
        }
    }
    else
    {
        // User closed the dialog, so don't error out.
        return;
    }
}

bool
ImageViewer::loadImage(const QString &filename)
{
    myLineEdit->setText(filename);

    if (!myLoadedImage.load(filename))
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
    QuantizeTask *task = new QuantizeTask(
        image, myPaletteDisplaySize, Quantizer::Method::MedianCut
    );

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
    myPalette = std::move(palette);
    SickLogger::log("Filled color palette");

    // override orig with processed to make sure
    // result stays the same when we resize
    QImage image = getImage();
    myLoadedImage = QPixmap::fromImage(image);

    myImageHolder->setPixmap(resizeImage(&myLoadedImage));
    emit tellBossAboutPaletteFill(&myPalette);

    myProcessorButton->setEnabled(true);
}

void
ImageViewer::applyPalette(QList<QColor> *palette)
{
    myProcessorButton->setEnabled(false);

    SickLogger::log("Applying color palette");

    QImage image = getImage();

    auto device = PaletteProcessorDevice(myDeviceDropdown->item());
    SickLogger::log(QString("Using: %1").arg(getDeviceStr(device)));

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
    SickLogger::log("Done applying color palette");

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
ImageViewer::setPaletteDisplaySize(int size)
{
    myPaletteDisplaySize = size;
}

QList<QColor> *
ImageViewer::palette()
{
    return &myPalette;
}
