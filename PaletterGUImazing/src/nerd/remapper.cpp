#include "remapper.h"
#include "sick_logger.h"

// THIS CAN'T BE INCLUDED BECAUSE NVCC DOESNT LIKE IT !!!!!!
// #include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QRgb>
#include <QtMath>

/*
 TODO: To avoid ifdef-ing all over the place we need to create a compute
 library that abstracts platform-specific backends. So then the image
 processor would only use the compute library, which would internally select
 CUDA, Metal, etc. Epic ASCII chart:

                                 CUDA
                                /
 Image processor <- GPU compute - Metal
             \                  \
              \                   etc.
               CPU compute

 It would be a bunch of work but it would be a neat sub-project. But for now
 ifdef spam is fine
*/

#ifdef USE_CUDA
#include "zoom.cuh"
#include <cuda_runtime.h>
#endif

Remapper::Remapper(CompareMethod method, const QList<QColor> &palette)
    : myCompareMethod(method), myPalette(palette)
{
}

void
Remapper::remap(QImage &image) const
{
#ifdef USE_CUDA

    int deviceCount = 0;
    cudaError_t err = cudaGetDeviceCount(&deviceCount);

    if (err == cudaSuccess && deviceCount > 0)
    {
        SickLogger::log("Using CUDA !");
        for (int dev = 0; dev < deviceCount; ++dev) 
        {
            cudaDeviceProp deviceProp;
            cudaGetDeviceProperties(&deviceProp, dev);
            QString dev_name(deviceProp.name);
            SickLogger::log(QString("Device %1").arg(dev_name));
        }
    
    }
    else
    {
        SickLogger::log("Failed to find a CUDA device !!",
                        SickLogSeverity::ERROR);
        return;
    }

    int width = image.width();
    int height = image.height();
    int pixel_count = width * height;

    QVector<float3> v_palette;
    v_palette.reserve(myPalette.size());

    for (int i = 0; i < myPalette.size(); ++i)
    {
        float paletter, paletteg, paletteb;

        myPalette.at(i).getRgbF(&paletter, &paletteg, &paletteb);

        v_palette.push_back(make_float3(paletter, paletteg, paletteb));
    }

    float3 *cu_palette;
    cudaMalloc(&cu_palette, v_palette.size() * sizeof(float3));

    cudaMemcpy(
        cu_palette,
        v_palette.data(),
        v_palette.size() * sizeof(float3),
        cudaMemcpyHostToDevice
    );

    uchar4 *cu_image;
    cudaMalloc(&cu_image, pixel_count * sizeof(uchar4));
    cudaMemcpy(
        cu_image,
        image.bits(),
        pixel_count * sizeof(uchar4),
        cudaMemcpyHostToDevice
    );

    Zoom::applyPaletteByLength(
        cu_image, width, height, cu_palette, v_palette.size()
    );

    cudaMemcpy(
        image.bits(),
        cu_image,
        pixel_count * sizeof(uchar4),
        cudaMemcpyDeviceToHost
    );

    cudaFree(cu_palette);
    cudaFree(cu_image);

    // TODO: #elif USE_METAL

#else
    // Just forward to host
    remapHost(image);
#endif
}

void
Remapper::remapHost(QImage &image) const
{
    float threshold = 0.01;

    for (int y = 0; y < image.height(); ++y)
    {
        QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y));

        for (int x = 0; x < image.width(); ++x)
        {
            QRgb &rgb = line[x];
            QRgb result = rgb;

            float paletter, paletteg, paletteb;

            float delta = 1000.0;

            for (int i = 0; i < myPalette.size(); ++i)

            {
                QColor currcolor = QColor(rgb);
                myPalette.at(i).getRgbF(&paletter, &paletteg, &paletteb);

                float length_delta = qSqrt(
                    qPow(paletter - currcolor.redF(), 2) +
                    qPow(paletteg - currcolor.greenF(), 2) +
                    qPow(paletteb - currcolor.blueF(), 2)
                );
                if (length_delta < delta)
                {
                    // this is stupid
                    result =
                        QColor::fromRgbF(paletter, paletteg, paletteb).rgb();
                    // update delta
                    delta = length_delta;
                }
                if (delta < threshold)
                    break;  // optimization
            }
            rgb = result;
        }
    }
}
