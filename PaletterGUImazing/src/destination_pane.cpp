#include "destination_pane.h"

#include <QMessageBox>
#include <QtWidgets>

#include "baller_task.h"
#include "nerd_types.h"
#include "sick_image_viewer.h"
#include "sick_logger.h"

static const int theInitialScaleFactor = 2;

DestinationPane::DestinationPane(QWidget *parent) : QWidget(parent)
{
    // my boy Ethan so good lookin'
    // no need to specify mode as it is READ by default
    myLineEdit = new SickFileLineEdit(this, tr("Ethan so sexy"));
    // load image when user has loaded image through file chooser
    connect(
        myLineEdit,
        &SickFileLineEdit::tellBossAboutFileLoaded,
        this,
        &DestinationPane::loadImage
    );
    // user can also type image in, try to load after they're done editing
    // line edit
    connect(
        myLineEdit->lineEdit(),
        &QLineEdit::editingFinished,
        this,
        &DestinationPane::onLoadImageFromLineEdit
    );

    myResetButton = new QPushButton("Reset");
    connect(
        myResetButton,
        &QPushButton::clicked,
        this,
        &DestinationPane::resetImage
    );
    myResetButton->setEnabled(false);

    myApplyPaletteButton = new QPushButton(tr("Apply Palette"));
    connect(
        myApplyPaletteButton,
        &QPushButton::clicked,
        this,
        &DestinationPane::askForPalette
    );
    myApplyPaletteButton->setEnabled(false);

    myRemapMethodDropdown = new SickDropDown(this, tr("Mode"));
    myRemapMethodDropdown->addMenuItem(tr("Distance"));
    myRemapMethodDropdown->addMenuItem(tr("Luminance"));
    myRemapMethodDropdown->addMenuItem(tr("Hue"));
    myRemapMethodDropdown->addMenuItem(tr("Saturation"));

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
    toolbar->addWidget(myApplyPaletteButton);
    toolbar->addWidget(myRemapMethodDropdown);
    toolbar->addWidget(myDeviceDropdown);

    myImageViewer = new SickImageViewer(this);

    // main layout
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    layout->addLayout(toolbar);
    layout->addWidget(myImageViewer);

    setLayout(layout);

    // Enable drag-and-drop support.
    setAcceptDrops(true);
}

void
DestinationPane::loadImage(const QString &file_path)
{
    if (!myUnfilteredImage.load(file_path))
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

    myApplyPaletteButton->setEnabled(true);
    myResetButton->setEnabled(true);

    // send a message in log about image being loaded
    const QString native_path = QDir::toNativeSeparators(file_path);
    QString message = QString("Loaded Image: %1").arg(native_path);
    SickLogger::log(message, SickLogSeverity::SEL);
}

void
DestinationPane::onLoadImageFromLineEdit()
{
    loadImage(myLineEdit->text());
}

void
DestinationPane::applyPalette(QList<QColor> *palette)
{
    myApplyPaletteButton->setEnabled(false);

    SickLogger::log("Applying color palette");

    const BallerDevice device = BallerDevice(myDeviceDropdown->item());
    SickLogger::log(QString("Using: %1").arg(getDeviceStr(device)));

    const NerdCompareMethod method = NerdCompareMethod(myRemapMethodDropdown->item());

    // TODO: the task should be hidden behind an ImageProcessor interface.
    // the ImageViewer should not create threads or tasks directly.
    RemapTask *task = new RemapTask(myUnfilteredImage, device, method, *palette);

    connect(
        task,
        &RemapTask::finished,
        this,
        &DestinationPane::onApplyPaletteFinished
    );

    QThread *thread = new QThread();
    task->runOnThread(thread);
}

void
DestinationPane::onApplyPaletteFinished(QImage image)
{
    SickLogger::log("Done applying color palette");

    myImage = image.copy();
    myImageViewer->setImage(myImage);

    myApplyPaletteButton->setEnabled(true);
}

void
DestinationPane::askForPalette()
{
    emit askBossForPalette();
}

void
DestinationPane::paintEvent(QPaintEvent *event)
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
DestinationPane::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls() && (event->possibleActions() & Qt::CopyAction))
    {
        event->acceptProposedAction();
    }
}

void
DestinationPane::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    const QUrl &url = urls.first();
    loadImage(url.path());
}

void
DestinationPane::resetImage()
{
    if (myUnfilteredImage.isNull())
        return;

    myImage = myUnfilteredImage.copy();

    myImageViewer->setImage(myImage);
    myImageViewer->frameImage();
}
