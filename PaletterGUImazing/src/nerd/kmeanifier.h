#ifndef KMEANIFIER_H
#define KMEANIFIER_H

#include <QColor>
#include <QImage>
#include <QVector>

#ifdef USE_CUDA
#include <cuda_runtime.h>
#endif

class KMeanifier
{
public:
    KMeanifier(int palette_size);

    // does the kmeans to generate palette
    // trust me bro guarantee
    QVector<QColor> generatePalette(const QImage &image) const;

private:
#ifdef USE_CUDA
    void palettize(
        float3 *palette,
        const uchar4 *img,
        const int width,
        const int height,
        const int palette_size
    ) const;
#endif

    int myPaletteSize;
};

#endif  // KMEANIFIER_H
