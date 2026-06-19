#include "dst_image_viewer.h"

#include <QMessageBox>
#include <QtWidgets>

#include "baller_task.h"
#include "remapper.h"
#include "sick_image_viewer.h"
#include "sick_logger.h"

static const int theInitialScaleFactor = 2;

DstImageViewer::DstImageViewer(QWidget *parent) : QWidget(parent)
{
    // my boy Ethan so good lookin'
    // no need to specify mode as it is READ by default
    myLineEdit = new SickFileLineEdit(this, tr("Ethan so sexy"));
    // load image when user has loaded image through file chooser
    connect(
        myLineEdit,
        &SickFileLineEdit::tellBossAboutFileLoaded,
        this,
        &DstImageViewer::onLoadImage
    );
    // user can also type image in, try to load after they're done editing
    // line edit
    connect(
        myLineEdit->lineEdit(),
        &QLineEdit::editingFinished,
        this,
        &DstImageViewer::onLoadImageFromLineEdit
    );

    myImageViewer = new SickImageViewer(this);

    myResetButton = new QPushButton("Reset");
    connect(
        myResetButton,
        &QPushButton::clicked,
        this,
        &DstImageViewer::resetImage
    );
    myResetButton->setEnabled(false);

    myProcessorButton = new QPushButton(tr("Apply Palette"));
    connect(
        myProcessorButton,
        &QPushButton::clicked,
        this,
        &DstImageViewer::askForPalette
    );
    myProcessorButton->setEnabled(false);

    myModeDropdown = new SickDropDown(this, tr("Mode"));
    myModeDropdown->addMenuItem(tr("Distance"));
    myModeDropdown->addMenuItem(tr("Luminance"));
    myModeDropdown->addMenuItem(tr("Hue"));
    myModeDropdown->addMenuItem(tr("Saturation"));

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

    // toolbar layout
    QHBoxLayout *toolbar = new QHBoxLayout();
    toolbar->addWidget(myLineEdit);
    toolbar->addWidget(myResetButton);
    toolbar->addWidget(myProcessorButton);
    toolbar->addWidget(myModeDropdown);
    toolbar->addWidget(myDeviceDropdown);

    // main layout
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    layout->addLayout(toolbar);
    layout->addWidget(myImageViewer);

    setLayout(layout);
}

void
DstImageViewer::onLoadImage(const QString &filename)
{
    if (!myUnfilteredImage.load(filename))
    {
        QString message = "Failed to load image file";
        SickLogger::log(message, SickLogSeverity::ERROR);
        QMessageBox::information(
            this, QGuiApplication::applicationDisplayName(), message
        );
        return;
    }

    myImage = myUnfilteredImage.copy();
    myImageViewer->setImage(myImage);
    myImageViewer->frameImage();

    myProcessorButton->setEnabled(true);
    myResetButton->setEnabled(true);

    // send a message in log about image being loaded
    const QString native_path = QDir::toNativeSeparators(filename);
    QString message = QString("Loaded Image: %1").arg(native_path);
    SickLogger::log(message, SickLogSeverity::SEL);
}

void
DstImageViewer::onLoadImageFromLineEdit()
{
    onLoadImage(myLineEdit->text());
}

void
DstImageViewer::applyPalette(QList<QColor> *palette)
{
    myProcessorButton->setEnabled(false);

    SickLogger::log("Applying color palette");

    auto device = PaletteProcessorDevice(myDeviceDropdown->item());
    SickLogger::log(QString("Using: %1").arg(getDeviceStr(device)));

    auto method = Remapper::CompareMethod(myModeDropdown->item());

    // TODO: the task should be hidden behind an ImageProcessor interface.
    // the ImageViewer should not create threads or tasks directly.
    RemapTask *task = new RemapTask(myImage, device, method, *palette);

    connect(
        task,
        &RemapTask::finished,
        this,
        &DstImageViewer::onApplyPaletteFinished
    );

    QThread *thread = new QThread();
    task->runOnThread(thread);
}

void
DstImageViewer::onApplyPaletteFinished(QImage image)
{
    SickLogger::log("Done applying color palette");

    myImage = image.copy();
    myImageViewer->setImage(myImage);

    myProcessorButton->setEnabled(true);
}

void
DstImageViewer::askForPalette()
{
    emit askBossForPalette();
}

void
DstImageViewer::paintEvent(QPaintEvent *event)
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
DstImageViewer::resetImage()
{
    if (myUnfilteredImage.isNull())
        return;

    myImage = myUnfilteredImage.copy();

    myImageViewer->setImage(myImage);
    myImageViewer->frameImage();
}
