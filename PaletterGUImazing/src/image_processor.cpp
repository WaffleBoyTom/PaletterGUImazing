#include "image_processor.h"

#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QRgb>
#include <QtMath>

#include "nerd/kmeanifier.h"
#include "nerd/median_cut.h"
#include "nerd/remapper.h"

QVector<QColor>
imageProcessorCreateColorPalette(
    const QImage &image, int palette_size, PaletteAlgorithm algorithm
)
{
    switch (algorithm)
    {
    case PaletteAlgorithm::MedianCut:
        return MedianCut(palette_size).generatePalette(image);
    case PaletteAlgorithm::KMeans:
        return KMeanifier(palette_size).generatePalette(image);
    }

    return {};
}

QImage
imageProcessorApplyColorPalette(
    QImage image,
    QList<QColor> *palette,
    ExecutionProvider provider,
    CompareMethod method
)
{
    Remapper remapper(method, *palette);

    switch (provider)
    {
    case ExecutionProvider::CPU:
    {
        remapper.remapHost(image);
        break;
    }
    case ExecutionProvider::GPU:
    {
        remapper.remap(image);
        break;
    }
    }

    return image;
}
