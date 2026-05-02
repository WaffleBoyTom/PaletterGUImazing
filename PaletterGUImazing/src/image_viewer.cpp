#include "image_viewer.h"

#include <QMessageBox>
#include <QtWidgets>

#include "baller_task.h"
#include "sick_logger.h"

// TODO: scaling factor hardcoded to 1/2 right now
// might wanna change that innit
static const int theImageScaleFactor = 2;

ImageViewer::ImageViewer(QWidget *parent, bool paletteSource = true)
    : QWidget(parent)
{
    // main layout
    myLayout = new QVBoxLayout();
    myLayout->setAlignment(Qt::AlignTop);

    // my boy Ethan so good lookin'
    // no need to specify mode as it is READ by default
    myLineEdit = new SickFileLineEdit(this, tr("Ethan so sexy"));
    // load image when user has loaded image through file chooser
    connect(
        myLineEdit,
        &SickFileLineEdit::tellBossAboutFileLoaded,
        this,
        &ImageViewer::loadImage
    );
    // user can also type image in, try to load after they're done editing
    // line edit
    connect(
        myLineEdit->lineEdit(),
        &QLineEdit::editingFinished,
        this,
        &ImageViewer::loadImageFromLineEdit
    );

    // image holder
    myImageHolder = new SickImageHolder(nullptr, tr(""));
    myImageHolder->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    connect(
        myImageHolder,
        &SickImageHolder::tellBossToResize,
        this,
        &ImageViewer::resizeOnDrag
    );

    // processor button
    // temp solution to allow for a different button
    // between the two image viewers
    // FIXME: we want a base class for these two viewers
    // with overrides so we don't do this nasty if stuff ?
    auto dropdowns = new QHBoxLayout();

    myModeDropdown = new SickDropDown(this, tr("Mode"));

    // TODO: inheritanceizationify this instead of using a flag lol
    if (paletteSource)
    {
        myProcessorButton = new QPushButton(tr("Process Image"));

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
        myProcessorButton = new QPushButton(tr("Apply Palette to Image"));

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
    myDeviceDropdown->addMenuItem(tr("Metal"));
#elif defined(USE_CUDA)
    myDeviceDropdown->addMenuItem(tr("CUDA"));
    // if we compile with CUDA, then it should be the
    // the default as it is the better option !
    myDeviceDropdown->setMenuItem(1);
#endif

    setPaletteDisplaySize(INIT_PALETTE_SIZE);
    myPalette = QList<QColor>();

    // populate layout
    myLayout->addWidget(myLineEdit);
    // myLayout->addWidget(myNautilusButton);
    myLayout->addWidget(myProcessorButton);
    dropdowns->addWidget(myModeDropdown);
    dropdowns->addWidget(myDeviceDropdown);
    myLayout->addLayout(dropdowns);
    myLayout->addWidget(myImageHolder);

    setLayout(myLayout);
}

void
ImageViewer::loadImage(const QString &filename)
{
    if (!myLoadedImage.load(filename))
    {
        QString message = "Failed to load image file";
        SickLogger::log(message, SickLogSeverity::ERROR);
        QMessageBox::information(
            this, QGuiApplication::applicationDisplayName(), message
        );
        return;
    }

    myProcessorButton->setEnabled(true);

    handleResizing();

    // send a message in log about image being loaded

    const QString native_path = QDir::toNativeSeparators(filename);
    QString message = QString("Loaded Image: %1").arg(native_path);
    SickLogger::log(message, SickLogSeverity::SEL);
}

void
ImageViewer::loadImageFromLineEdit()
{
    loadImage(myLineEdit->text());
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
    {
        // button should be greyed out if no image has been loaded
        // so this should never happen
        Q_ASSERT("How the fuck did we get here !");
        return;
    }
    myProcessorButton->setEnabled(false);

    // TODO: the task should be hidden behind an ImageProcessor interface.
    // the ImageViewer should not create threads or tasks directly.
    QImage image = getImage();
    QuantizeTask *task = new QuantizeTask(
        image, myPaletteDisplaySize, Quantizer::Method(myModeDropdown->item())
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

    // myImageHolder->setPixmap(resizeImage(&myLoadedImage));
    myImageHolder->setPixmap(resizeImage(
        &myLoadedImage,
        myImageHolder->pixmap().width(),
        myImageHolder->pixmap().height()
    ));
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

    // don't override original !
    // myLoadedImage = QPixmap::fromImage(image);
    QPixmap pixmap = QPixmap::fromImage(image);
    myImageHolder->setPixmap(resizeImage(
        &pixmap,
        myImageHolder->pixmap().width(),
        myImageHolder->pixmap().height()
    ));

    myProcessorButton->setEnabled(true);
}

void
ImageViewer::askForPalette()
{
    emit askBossForPalette();
}

QPixmap
ImageViewer::resizeImage(QPixmap *imagedisplay, int width, int height)
{
    // by default images are pretty big
    // unlike other things...
    // this probably should scale based on
    // main window size
    // dividing by 2 for now, idk

    return imagedisplay->scaled(
        width, height, Qt::KeepAspectRatio /* ar */
    );
}

void
ImageViewer::handleResizing()
{
    if (myLoadedImage.isNull())
        return;

    const QPixmap scaled = resizeImage(
        &myLoadedImage,
        parentWidget()->height() / theImageScaleFactor, /* width */
        parentWidget()->width() / theImageScaleFactor   /* height */
    );

    myImageHolder->setPixmap(scaled);
}

void
ImageViewer::resizeOnDrag(int width, int height)
{
    if (myLoadedImage.isNull())
        return;

    // FIXME: this means we override an image which has been
    // paletted : (
    const QPixmap scaled = resizeImage(&myLoadedImage, width, height);

    myImageHolder->setPixmap(scaled);
}

void
ImageViewer::paintEvent(QPaintEvent *event)
{
    // FIXME : It'd be lit if we could
    // drag a rectangle around the image
    // when it's being dragged !
    // QPainter painter(this);
    // const QPoint topleft = myImageHolder->rect().center();
    // painter.drawRect(
    //     topleft.x(),
    //     topleft.y(),
    //     myImageHolder->width(),
    //     myImageHolder->height()
    // );
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
