#include "dst_image_viewer.h"

#include <QMessageBox>
#include <QtWidgets>

#include "baller_task.h"
#include "sick_logger.h"
#include "remapper.h"

// TODO: scaling factor hardcoded to 1/2 right now
// might wanna change that innit
static const int theImageScaleFactor = 2;

DstImageViewer::DstImageViewer(QWidget *parent)
    : QWidget(parent)
{
    // main layout
    myLayout = new QVBoxLayout();
    myLayout->setAlignment(Qt::AlignTop);

    
    // my boy Ethan so good lookin'
    // no need to specify mode as it is READ by default 
    myLineEdit = new SickFileLineEdit(
        this,
        tr("Ethan so sexy")
    );
    // load image when user has loaded image through file chooser
    connect(
        myLineEdit,
        &SickFileLineEdit::tellBossAboutFileLoaded,
        this,
        &DstImageViewer::loadImage    
    );
    // user can also type image in, try to load after they're done editing
    // line edit
    connect(
        myLineEdit->lineEdit(),
        &QLineEdit::editingFinished,
        this,
        &DstImageViewer::loadImageFromLineEdit
    );

    // image holder
    myImageHolder = new SickImageHolder(nullptr, tr(""));
    myImageHolder->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    connect(
        myImageHolder,
        &SickImageHolder::tellBossToResize,
        this,
        &DstImageViewer::resizeOnDrag
    );

    auto dropdowns = new QHBoxLayout();

    myModeDropdown = new SickDropDown(this, tr("Mode"));

    myProcessorButton = new QPushButton(tr("Apply Palette to Image"));

    connect(
        myProcessorButton,
        &QPushButton::clicked,
        this,
        &DstImageViewer::askForPalette
    );

    myProcessorButton->setEnabled(false);

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
DstImageViewer::loadImage(const QString &filename)
{
    if (!myLoadedImage.load(filename))
    {
        QString message = "Failed to load image file";
        SickLogger::log(message, SickLogSeverity::ERROR);
        QMessageBox::information(
            this, 
            QGuiApplication::applicationDisplayName(), 
            message
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
DstImageViewer::loadImageFromLineEdit()
{
    loadImage(myLineEdit->text());
}


QImage
DstImageViewer::getImage()
{
    // we want to load in the original image
    // not the resized one
    return myLoadedImage.toImage();
}

void
DstImageViewer::applyPalette(QList<QColor> *palette)
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

    // don't override original !
    // myLoadedImage = QPixmap::fromImage(image);
    QPixmap pixmap = QPixmap::fromImage(image);

    // keep the modified image around.
    myModifiedImage = pixmap;
    
    myImageHolder->setPixmap(
        resizeImage(
            &pixmap,            
            myImageHolder->pixmap().width(),
            myImageHolder->pixmap().height()
        )
    );

    myProcessorButton->setEnabled(true);
}

void
DstImageViewer::askForPalette()
{
    emit askBossForPalette();
}

QPixmap
DstImageViewer::resizeImage(QPixmap *imagedisplay, 
                         int width, int height)
{
    // by default images are pretty big
    // unlike other things...
    // this probably should scale based on
    // main window size
    // dividing by 2 for now, idk

    return imagedisplay->scaled(
        width, height,
        Qt::KeepAspectRatio /* ar */
    );
}

void
DstImageViewer::handleResizing()
{
    // we have nothing, do nothing
    if (myLoadedImage.isNull() && myModifiedImage.isNull())
        return;

    const QPixmap scaled = resizeImage(
        !myModifiedImage.isNull() ? &myModifiedImage : &myLoadedImage,
        parentWidget()->height() / theImageScaleFactor, /* width */
        parentWidget()->width() / theImageScaleFactor  /* height */
    );

    myImageHolder->setPixmap(scaled);
}

void
DstImageViewer::resizeOnDrag(int width, int height)
{
    if (myLoadedImage.isNull() && myModifiedImage.isNull())
        return;

    
    // FIXME: this means we override an image which has been
    // paletted : ( 
    const QPixmap scaled = resizeImage(
        !myModifiedImage.isNull() ? &myModifiedImage : &myLoadedImage,
        width,
        height    
    );
    
    myImageHolder->setPixmap(scaled);

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

