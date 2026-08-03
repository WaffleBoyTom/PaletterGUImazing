#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <QColor>
#include <QImage>
#include <QList>
#include <QPixmap>

#include "nerd/nerd_types.h"

enum class ExecutionProvider
{
    CPU = 0,
    GPU,
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
    const QImage &image, int palette_size, NerdPaletteAlgorithm algorithm
);

QImage imageProcessorApplyColorPalette(
    QImage image,
    QList<QColor> *palette,
    ExecutionProvider provider,
    NerdCompareMethod method
);

#endif  // IMAGE_PROCESSOR_H
