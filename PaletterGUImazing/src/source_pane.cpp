#include "source_pane.h"

#include <QMessageBox>
#include <QtConcurrent>
#include <QtWidgets>

#include "cool/cool_logger.h"
#include "image_processor.h"

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
        &SourcePane::loadImage
    );
    // user can also type image in, try to load after they're done editing
    // line edit
    connect(
        myLineEdit->lineEdit(),
        &QLineEdit::editingFinished,
        this,
        &SourcePane::onLoadImageFromLineEdit
    );

    myProcessorButton = new QPushButton(tr("Create Palette"));
    connect(
        myProcessorButton,
        &QPushButton::clicked,
        this,
        &SourcePane::generatePalette
    );
    myProcessorButton->setEnabled(false);

    myAlgorithmDropdown = new SickDropDown(this, tr("Mode"));
    myAlgorithmDropdown->addMenuItem(tr("Median Cut"));
    myAlgorithmDropdown->addMenuItem(tr("K-Means"));

    myExecutionProviderDropdown = new SickDropDown(this, tr("Provider"));
    myExecutionProviderDropdown->addMenuItem(tr("CPU"));

#if defined(USE_METAL)
    myExecutionProviderDropdown->addMenuItem(tr("Metal"));
#elif defined(USE_CUDA)
    myExecutionProviderDropdown->addMenuItem(tr("CUDA"));
    // if we compile with CUDA, then it should be the
    // the default as it is the better option !
    myExecutionProviderDropdown->setMenuItem(1);
    myAlgorithmDropdown->setMenuItem(1);
#endif

    myImageViewer = new SickImageViewer(this);

    setPaletteDisplaySize(INIT_PALETTE_SIZE);
    myPalette = QList<QColor>();

    // populate layout
    QHBoxLayout *toolbar = new QHBoxLayout();
    toolbar->addWidget(myLineEdit);
    toolbar->addWidget(myProcessorButton);
    toolbar->addWidget(myAlgorithmDropdown);
    toolbar->addWidget(myExecutionProviderDropdown);

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
SourcePane::loadImage(const QString &file_path)
{
    if (!myImage.load(file_path))
    {
        QString message = "Failed to load image file";
        CoolLogger::log(message, CoolLogSeverity::ERROR);
        QMessageBox::information(
            this, QGuiApplication::applicationDisplayName(), message
        );
        return;
    }

    myImageViewer->setImage(myImage);
    myImageViewer->frameImage();

    myProcessorButton->setEnabled(true);

    // send a message in log about image being loaded
    const QString native_path = QDir::toNativeSeparators(file_path);
    QString message = QString("Loaded Image: %1").arg(native_path);
    CoolLogger::log(message, CoolLogSeverity::SEL);
}

void
SourcePane::onLoadImageFromLineEdit()
{
    loadImage(myLineEdit->text());
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
    myProcessorButton->clearFocus();
    myProcessorButton->setEnabled(false);

    QFuture<QList<QColor>> future = QtConcurrent::run(
        [=]
        {
            return imageProcessorCreateColorPalette(
                myImage,
                myPaletteDisplaySize,
                NerdPaletteAlgorithm(myAlgorithmDropdown->item())
            );
        }
    );

    QFutureWatcher<QList<QColor>> *watcher =
        new QFutureWatcher<QList<QColor>>(this);

    connect(
        watcher,
        &QFutureWatcher<QList<QColor>>::finished,
        this,
        [this, watcher]
        {
            onGeneratePaletteFinished(watcher->result());
            watcher->deleteLater();
        }
    );

    QApplication::setOverrideCursor(Qt::WaitCursor);

    connect(
        watcher,
        &QFutureWatcher<QList<QColor>>::finished,
        this,
        [this, watcher]
        {
            QApplication::restoreOverrideCursor();

            onGeneratePaletteFinished(watcher->result());

            watcher->deleteLater();
        }
    );

    watcher->setFuture(future);
}

void
SourcePane::onGeneratePaletteFinished(QList<QColor> palette)
{
    CoolLogger::log("Filled color palette");

    myPalette = std::move(palette);
    emit tellBossAboutPaletteFill(&myPalette);

    myProcessorButton->setEnabled(true);
}

void
SourcePane::paintEvent(QPaintEvent *event)
{
    // FIXME: when is being inspected is set to True we should trigger
    // a repaint !
    if (myImageViewer->isBeingInspected())
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // FIXME : we should have an easy way to
        // get these colors ..
        QBrush brush(QColorConstants::Svg::cornsilk);
        painter.setBrush(brush);
        QPen pen(QColorConstants::Svg::cornsilk);
        pen.setWidth(3);
        painter.setPen(pen);
        QFont font = painter.font();
        font.setPointSize(10);
        painter.setFont(font);
        QString info = QString(" Resolution: %1-%2")
                           .arg(myImage.width())
                           .arg(myImage.height());
        QRect r = rect();
        QPoint br = r.bottomLeft();
        painter.drawText(br, info);
    }
}

void
SourcePane::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls() &&
        (event->possibleActions() & Qt::CopyAction))
    {
        event->acceptProposedAction();
    }
}

void
SourcePane::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    const QUrl &url = urls.first();
    loadImage(url.path());
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
