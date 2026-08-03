#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <QColor>
#include <QImage>
#include <QList>
#include <QPixmap>

#include "nerd/remapper.h"  // for CompareMethod

enum class ExecutionProvider
{
    CPU = 0,
    GPU,
};

enum class PaletteAlgorithm
{
    MedianCut = 0,
    KMeans
};

inline const char *
executionProviderString(ExecutionProvider provider)
{
    switch (provider)
    {
    case ExecutionProvider::CPU:
        return "CPU";
    case ExecutionProvider::GPU:
        return "GPU";
    default:
        Q_ASSERT("How did we get here !!");
    }
    return "UNDEFINED";
}

QVector<QColor> imageProcessorCreateColorPalette(
    const QImage &image, int palette_size, PaletteAlgorithm algorithm
);

QImage imageProcessorApplyColorPalette(
    QImage image,
    QList<QColor> *palette,
    ExecutionProvider provider,
    CompareMethod method
);

#endif  // IMAGE_PROCESSOR_H
