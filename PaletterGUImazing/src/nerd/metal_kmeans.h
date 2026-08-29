#ifndef KMEANS_METAL_H
#define KMEANS_METAL_H

#include <QColor>
#include <QImage>
#include <QList>

namespace MTL
{
class ComputePipelineState;
class Buffer;
}

class KMeansMetal
{
public:
    KMeansMetal(const QImage &image, int k);

    QList<QColor> cluster(int iterations);

private:
    void accumulate();
    void resolve();

    QSize myResolution;
    std::size_t myK;

    // Metal-specific.
    MTL::ComputePipelineState *myKmeansNaiveAccumulatePipeline;
    MTL::ComputePipelineState *myKmeansNaiveResolvePipeline;

    MTL::Buffer *myImage;
    MTL::Buffer *myMeans;
    MTL::Buffer *myAccumulators;
};

#endif  // KMEANS_METAL_H
