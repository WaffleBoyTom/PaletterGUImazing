#include "source_pane.h"

#include <QMessageBox>
#include <QtWidgets>

#include "baller_task.h"
#include "sick_logger.h"

SourcePane::SourcePane(QWidget *parent) : QWidget(parent)
{
    // my man Tom so good lookin'
    // no need to specify mode as it is READ by default
    myLineEdit = new SickFileLineEdit(this, tr("Tom so sexy"));
    // load image when user has loaded image through file chooser
    connect(
        myLineEdit,
        &SickFileLineEdit::tellBossAboutFileLoaded,
        this,
        &SourcePane::onLoadImage
    );
    // user can also type image in, try to load after they're done editing
    // line edit
    connect(
        myLineEdit->lineEdit(),
        &QLineEdit::editingFinished,
        this,
        &SourcePane::onLoadImageFromLineEdit
    );

    myProcessorButton = new QPushButton(tr("Process Image"));
    connect(
        myProcessorButton,
        &QPushButton::clicked,
        this,
        &SourcePane::generatePalette
    );
    myProcessorButton->setEnabled(false);

    myQuantizationMethodDropdown = new SickDropDown(this, tr("Mode"));
    myQuantizationMethodDropdown->addMenuItem(tr("Median Cut"));
    myQuantizationMethodDropdown->addMenuItem(tr("K-Means"));

    myDeviceDropdown = new SickDropDown(this, tr("Device"));
    myDeviceDropdown->addMenuItem(tr("CPU"));

#if defined(USE_METAL)
    myDeviceDropdown->addMenuItem(tr("Metal"));
#elif defined(USE_CUDA)
    myDeviceDropdown->addMenuItem(tr("CUDA"));
    // if we compile with CUDA, then it should be the
    // the default as it is the better option !
    myDeviceDropdown->setMenuItem(1);
    myQuantizationMethodDropdown->setMenuItem(1);
#endif

    myImageViewer = new SickImageViewer(this);

    setPaletteDisplaySize(INIT_PALETTE_SIZE);
    myPalette = QList<QColor>();

    // populate layout
    QHBoxLayout *toolbar = new QHBoxLayout();
    toolbar->addWidget(myLineEdit);
    toolbar->addWidget(myProcessorButton);
    toolbar->addWidget(myQuantizationMethodDropdown);
    toolbar->addWidget(myDeviceDropdown);

    // main layout
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    layout->addLayout(toolbar);
    layout->addWidget(myImageViewer);

    setLayout(layout);
}

void
SourcePane::onLoadImage(const QString &filename)
{
    if (!myImage.load(filename))
    {
        QString message = "Failed to load image file";
        SickLogger::log(message, SickLogSeverity::ERROR);
        QMessageBox::information(
            this, QGuiApplication::applicationDisplayName(), message
        );
        return;
    }

    myImageViewer->setImage(myImage);
    myImageViewer->frameImage();

    myProcessorButton->setEnabled(true);

    // send a message in log about image being loaded
    const QString native_path = QDir::toNativeSeparators(filename);
    QString message = QString("Loaded Image: %1").arg(native_path);
    SickLogger::log(message, SickLogSeverity::SEL);
}

void
SourcePane::onLoadImageFromLineEdit()
{
    onLoadImage(myLineEdit->text());
}

void
SourcePane::generatePalette()
{
    if (myImage.isNull())
    {
        // button should be greyed out if no image has been loaded
        // so this should never happen
        Q_ASSERT("How the fuck did we get here !");
        return;
    }
    myProcessorButton->setEnabled(false);

    // TODO: the task should be hidden behind an ImageProcessor interface.
    // the ImageViewer should not create threads or tasks directly.
    QuantizeTask *task = new QuantizeTask(
        myImage, myPaletteDisplaySize, Quantizer::Method(myQuantizationMethodDropdown->item())
    );

    connect(
        task,
        &QuantizeTask::finished,
        this,
        &SourcePane::onGeneratePaletteFinished
    );

    QThread *thread = new QThread();
    task->runOnThread(thread);
}

void
SourcePane::onGeneratePaletteFinished(QList<QColor> palette)
{
    SickLogger::log("Filled color palette");

    myPalette = std::move(palette);
    emit tellBossAboutPaletteFill(&myPalette);

    myProcessorButton->setEnabled(true);
}

void
SourcePane::paintEvent(QPaintEvent *event)
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
SourcePane::setPaletteDisplaySize(int size)
{
    myPaletteDisplaySize = size;
}

QList<QColor> *
SourcePane::palette()
{
    return &myPalette;
}
