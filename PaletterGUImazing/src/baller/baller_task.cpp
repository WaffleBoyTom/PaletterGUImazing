#include "baller_task.h"

BallerTask::BallerTask()
{
}

void
BallerTask::run()
{
    runInternal();
    emit _postCompletion();
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

QuantizeTask::QuantizeTask(
    QImage image, int palette_size, Quantizer::Method method
)
    : myImage(std::move(image)), myPaletteSize(palette_size), myMethod(method)
{
}

void
QuantizeTask::runInternal()
{
    QList<QColor> palette =
        Quantizer(myPaletteSize, myMethod).generatePalette(myImage);
    emit finished(palette);
}

RemapTask::RemapTask(
    QImage image,
    PaletteProcessorDevice device,
    Remapper::CompareMethod method,
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

    // stop going through the palette if we're within .05
    switch (myDevice)
    {
    case PaletteProcessorDevice::CPU:
    {
        remapper.remapHost(myImage);
        break;
    }
    case PaletteProcessorDevice::GPU:
    {
        remapper.remap(myImage);
        break;
    }
    }

    emit finished(myImage);
}
