#include "baller_task.h"
#include "kmeanifier.h"
#include "nerd_types.h"
#include "palette_generator.h"
#include "remapper.h"
#include <QApplication>

BallerTask::BallerTask()
{
}

void
BallerTask::setCursorBusy()
{
    // FIXME: This one is boring, we should make a custom one 
    QApplication::setOverrideCursor(Qt::WaitCursor);
}
void
BallerTask::restoreCursorToBoring()
{
    QApplication::restoreOverrideCursor();
}

void
BallerTask::run()
{
    setCursorBusy();
    runInternal();
    emit _postCompletion();
    restoreCursorToBoring();
}

void
BallerTask::runOnThread(QThread *thread)
{
    QObject::moveToThread(thread);

    connect(thread, &QThread::started, this, &BallerTask::run);
    connect(this, &BallerTask::_postCompletion, thread, &QThread::quit);
    connect(this, &BallerTask::_postCompletion, this, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    thread->start();
}

GeneratePaletteTask::GeneratePaletteTask(
    QImage image, int palette_size, NerdPaletteAlgorithm algorithm
)
    : myImage(std::move(image)), myPaletteSize(palette_size), myAlgorithm(algorithm)
{
}

void
GeneratePaletteTask::runInternal()
{
    QList<QColor> palette;
    switch (myAlgorithm)
    {
        case NerdPaletteAlgorithm::MedianCut:
        {
            palette = PaletteGenerator(myPaletteSize, myAlgorithm).generatePalette(myImage);
            break;
        }
        case NerdPaletteAlgorithm::KMeans:
        {
            palette = KMeanifier(myPaletteSize).generatePalette(myImage);
            break;
        }
    }
    emit finished(palette);
    
}

RemapTask::RemapTask(
    QImage image,
    BallerDevice device,
    NerdCompareMethod method,
    QList<QColor> palette
)
    : myImage(std::move(image)),
      myDevice(device),
      myMethod(method),
      myPalette(std::move(palette))
{
}

void
RemapTask::runInternal()
{
    Remapper remapper(myMethod, myPalette);

    switch (myDevice)
    {
    case BallerDevice::CPU:
    {
        remapper.remapHost(myImage);
        break;
    }
    case BallerDevice::GPU:
    {
        remapper.remap(myImage);
        break;
    }
    }

    emit finished(myImage);
}
