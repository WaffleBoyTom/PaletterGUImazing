#ifndef BALLER_WORKER_H
#define BALLER_WORKER_H

#include <QObject>
#include <QThread>

#include "processor_device.h"
#include "quantizer.h"
#include "remapper.h"

QT_BEGIN_NAMESPACE
class QObject;
QT_END_NAMESPACE

// TODO: make a task manager :O to cap the number of running tasks.
//  otherwise we can just endlessly create threads

// Task object. Subclassed to bundle a set of operations.
class BallerTask : public QObject
{
    Q_OBJECT
public:
    BallerTask();

    void runOnThread(QThread *thread);

protected:
    void run();
    virtual void runInternal() = 0;

signals:
    void _postCompletion();
};

class QuantizeTask : public BallerTask
{
    Q_OBJECT
public:
    QuantizeTask(QImage image, int palette_size, Quantizer::Method method);

signals:
    void finished(QList<QColor> palette);

protected:
    void runInternal() override;

private:
    QImage myImage;
    int myPaletteSize;
    Quantizer::Method myMethod;
};

class RemapTask : public BallerTask
{
    Q_OBJECT
public:
    RemapTask(
        QImage image,
        PaletteProcessorDevice device,
        Remapper::CompareMethod method,
        QList<QColor> palette
    );

signals:
    void finished(QImage remapped_image);

protected:
    void runInternal() override;

private:
    QImage myImage;
    PaletteProcessorDevice myDevice;
    Remapper::CompareMethod myMethod;
    QList<QColor> myPalette;
};

#endif  // BALLER_WORKER_H
