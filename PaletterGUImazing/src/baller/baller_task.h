#ifndef BALLER_TASK_H
#define BALLER_TASK_H

#include <QImage>
#include <QObject>
#include <QThread>

#include "baller_types.h"
#include "nerd_types.h"

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
    // changes cursor to busy to show user we're doing stuff..
    void setCursorBusy();
    // when task has been completed, restore cursor
    void restoreCursorToBoring();

signals:
    void _postCompletion();
};

class GeneratePaletteTask : public BallerTask
{
    Q_OBJECT
public:
    GeneratePaletteTask(QImage image, int palette_size, NerdPaletteAlgorithm algorithm);

signals:
    void finished(QList<QColor> palette);

protected:
    void runInternal() override;

private:
    QImage myImage;
    int myPaletteSize;
    NerdPaletteAlgorithm myAlgorithm;
};

class RemapTask : public BallerTask
{
    Q_OBJECT
public:
    RemapTask(
        QImage image,
        BallerDevice device,
        NerdCompareMethod method,
        QList<QColor> palette
    );

signals:
    void finished(QImage remapped_image);

protected:
    void runInternal() override;

private:
    QImage myImage;
    BallerDevice myDevice;
    NerdCompareMethod myMethod;
    QList<QColor> myPalette;
};

#endif  // BALLER_TASK_H
