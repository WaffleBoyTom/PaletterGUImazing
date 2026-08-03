#include "kmeanifier.h"

#ifdef USE_CUDA
#include <cuda_runtime.h>
#endif

KMeanifier::KMeanifier(int palette_size) : myPaletteSize(palette_size)
{
}

QVector<QColor>
KMeanifier::generatePalette(const QImage &image) const
{
#ifdef USE_CUDA

    // FIXME:
    // should this be in ImageViewer::applyPalette instead ?
    // this being here means nerd lib has to include sick lib
    // which maybe is fine but seems a bit sketch ...

    int deviceCount = 0;
    cudaError_t err = cudaGetDeviceCount(&deviceCount);

    if (err == cudaSuccess && deviceCount > 0)
    {
        CoolUtilLogger::log("Using CUDA !", SickLogSeverity::CUDA);
        for (int dev = 0; dev < deviceCount; ++dev)
        {
            cudaDeviceProp deviceProp;
            cudaGetDeviceProperties(&deviceProp, dev);
            QString dev_name(deviceProp.name);
            CoolUtilLogger::log(
                QString("Device %1").arg(dev_name), SickLogSeverity::CUDA
            );
        }
    }
    else
    {
        CoolUtilLogger::log(
            "Failed to find a CUDA device !!", SickLogSeverity::ERROR
        );
        /// FIXME
        return QVector<QColor>();
    }

    int width = image.width();
    int height = image.height();
    int pixel_count = width * height;

    // initialize the palette
    QVector<float3> v_palette;
    v_palette.reserve(myPaletteSize);

    // the one magic number to rule them all
    QRandomGenerator rng(67);

    // initialize our palette with random samples from the image
    for (int i = 0; i < myPaletteSize; ++i)
    {
        int x = rng.bounded(width);
        int y = rng.bounded(height);
        QColor sample = image.pixelColor(x, y);
        v_palette.push_back(
            make_float3(sample.redF(), sample.greenF(), sample.blueF())
        );
    }

    /// upload palette to device
    float3 *cu_palette;
    cudaMalloc(&cu_palette, v_palette.size() * sizeof(float3));
    cudaMemcpy(
        cu_palette,
        v_palette.data(),
        v_palette.size() * sizeof(float3),
        cudaMemcpyHostToDevice
    );

    /// upload image to device
    uchar4 *cu_image;
    cudaMalloc(&cu_image, pixel_count * sizeof(uchar4));
    cudaMemcpy(
        cu_image,
        image.bits(),
        pixel_count * sizeof(uchar4),
        cudaMemcpyHostToDevice
    );

    /// run KMeans

    // CuMeans::palettize(
    //     cu_palette,
    //     cu_image,
    //     width,
    //     height,
    //     myPaletteSize
    // );

    /// copy the palette back to host
    cudaMemcpy(
        v_palette.data(),
        cu_palette,
        myPaletteSize * sizeof(float3),
        cudaMemcpyDeviceToHost
    );

    cudaFree(cu_palette);
    cudaFree(cu_image);

    /// do we have to do this ?
    QVector<QColor> colors;
    colors.reserve(myPaletteSize);
    for (int i = 0; i < myPaletteSize; ++i)
    {
        colors.push_back(
            QColor::fromRgbF(v_palette[i].x, v_palette[i].y, v_palette[i].z)
        );
    }

    return colors;
#else
    return {};
#endif
}
