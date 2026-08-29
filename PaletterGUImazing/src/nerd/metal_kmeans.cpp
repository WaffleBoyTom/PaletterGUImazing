#include "metal_kmeans.h"

#include <QtGui/qrgb.h>

#include <cstdint>

#include "metal_context.h"
#include "metal_dispatcher.h"

namespace
{

struct ClusterAccumulator
{
    std::uint32_t r;
    std::uint32_t g;
    std::uint32_t b;
    std::uint32_t count;
};

}

KMeansMetal::KMeansMetal(const QImage &image, int k)
    : myResolution(image.size()), myK(k)
{
    MetalContext &context = MetalContext::instance();

    myKmeansNaiveAccumulatePipeline =
        context.createPipelineState("kmeansNaiveAccumulate");
    myKmeansNaiveResolvePipeline =
        context.createPipelineState("kmeansNaiveResolve");

    // Create and load the image buffer.
    const std::size_t image_res = image.width() * image.height();
    const std::size_t image_size = image_res * sizeof(QRgb);
    myImage = context.createSharedBuffer(image_size);

    QRgb *image_ptr = reinterpret_cast<QRgb *>(myImage->contents());
    Q_ASSERT(image_ptr != nullptr);

    for (std::size_t y = 0, height = image.height(); y < height; ++y)
    {
        for (std::size_t x = 0, width = image.width(); x < width; ++x)
        {
            QRgb pixel = image.pixel(x, y);
            image_ptr[y * width + x] = pixel;
        }
    }

    // Create the means buffer.
    const std::size_t means_size = myK * sizeof(QRgb);
    myMeans = context.createSharedBuffer(means_size);

    // Create the accumulator buffer.
    const std::size_t accumulator_size = myK * sizeof(ClusterAccumulator);
    myAccumulators = context.createSharedBuffer(accumulator_size);
}

QList<QColor>
KMeansMetal::cluster(int iterations)
{
    // TODO: improve initial seed?
    QRgb *means_ptr = reinterpret_cast<QRgb *>(myMeans->contents());
    Q_ASSERT(means_ptr != nullptr);

    for (std::size_t i = 0; i < myK; ++i)
    {
        const int c = static_cast<int>(
            (static_cast<float>(i) / static_cast<float>(myK)) * 255.0
        );
        const QRgb rgba = qRgba(c, c, c, 255);
        means_ptr[i] = rgba;
    }

    for (std::size_t iter = 0; iter < iterations; ++iter)
    {
        accumulate();
        resolve();
    }

    QRgb *palette_out = reinterpret_cast<QRgb *>(myMeans->contents());
    Q_ASSERT(palette_out != nullptr);

    QList<QColor> palette(myK);
    for (std::size_t i = 0; i < myK; ++i)
    {
        palette[i] = QColor(palette_out[i]);
    }

    return palette;
}

void
KMeansMetal::accumulate()
{
    MetalDispatcher dispatcher(myKmeansNaiveAccumulatePipeline);

    const std::size_t accumulator_size = myK * sizeof(ClusterAccumulator);
    ::memset(myAccumulators->contents(), 0, accumulator_size);

    dispatcher.bindBuffer(myImage, 0, 0);
    dispatcher.bindBuffer(myMeans, 0, 1);
    dispatcher.bindBuffer(myAccumulators, 0, 2);

    uint32_t k = static_cast<uint32_t>(myK);
    dispatcher.bindBytes(&k, sizeof(k), 3);

    const std::size_t pixel_count =
        myResolution.width() * myResolution.height();
    const MTL::Size grid_size(pixel_count, 1, 1);
    dispatcher.dispatch(grid_size);
}

void
KMeansMetal::resolve()
{
    MetalDispatcher dispatcher(myKmeansNaiveResolvePipeline);

    dispatcher.bindBuffer(myAccumulators, 0, 0);
    dispatcher.bindBuffer(myMeans, 0, 1);

    const MTL::Size grid_size(myK, 1, 1);
    dispatcher.dispatch(grid_size);
}
