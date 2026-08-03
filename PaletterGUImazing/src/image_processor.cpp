#include "image_processor.h"

#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QRgb>
#include <QtMath>

#include "nerd/kmeanifier.h"
#include "nerd/nerd_types.h"
#include "nerd/palette_generator.h"
#include "nerd/remapper.h"

QVector<QColor>
imageProcessorCreateColorPalette(
    const QImage &image, int palette_size, NerdPaletteAlgorithm algorithm
)
{
    switch (algorithm)
    {
    case NerdPaletteAlgorithm::MedianCut:
        return PaletteGenerator(palette_size, algorithm).generatePalette(image);
    case NerdPaletteAlgorithm::KMeans:
        return KMeanifier(palette_size).generatePalette(image);
    }

    return {};
}

QImage
imageProcessorApplyColorPalette(
    QImage image,
    QList<QColor> *palette,
    ExecutionProvider provider,
    NerdCompareMethod method
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
