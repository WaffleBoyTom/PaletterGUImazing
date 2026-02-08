#include "remapper.h"

#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QRgb>
#include <QtMath>

// TODO: To avoid ifdef-ing all over the place we need to create a compute
// library that abstracts platform-specific backends. So then the image
// processor would only use the compute library, which would internally select
// CUDA, Metal, etc. Epic ASCII chart:
//
//                                 CUDA
//                                /
// Image processor <- GPU compute - Metal
//             \                  \
//              \                   etc.
//               CPU compute
//
// It would be a bunch of work but it would be a neat sub-project. But for now
// ifdef spam is fine
#ifdef USE_CUDA
#include "zoom.cuh"
#endif

Remapper::Remapper(Colorspace colorspace, const QList<QColor> &palette)
    : myColorspace(colorspace), myPalette(palette)
{
}

#ifdef USE_CUDA
static void
testMaxSpeedApplyColorPalette(
    QImage &image, QList<QColor> *palette, PaletterUtils::PaletteApplyMode mode
)
{
    int N = 1 << 20;
    float *x, *y;

    // Allocate Unified Memory – accessible from CPU or GPU
    // what does that mean, you ask ??
    // well, if you're an idiot like me you spend 20 minutes printing a float
    // that's not getting incremented and the reason behind it is because you
    // created an array on the host device, sent it to cuda, cuda then made a
    // copy on the gpu and that memory was never copied back to the cpu... so
    // either you copy that memory back to host or you use this super fancy
    // cudaMallocManaged which does that for you
    // basically i was uploading stuff to the gpu but never downloading it
    // because i'm braindead :)

    // most of this code comes from add.cu which is a tutorial I did some time
    // ago that you can find on nvidia's website, iirc

    cudaMallocManaged(&x, N * sizeof(float));
    cudaMallocManaged(&y, N * sizeof(float));

    for (int i = 0; i < N; ++i)
    {
        x[i] = 1.0f;
        y[i] = 2.0f;
    }

    Zoom::test(N, x, y);

    for (int i = 0; i < 10; ++i)
    {
        qDebug() << y[i];
    }

    // Free memory
    cudaFree(x);
    cudaFree(y);
}
#endif

void
Remapper::remap(QImage &image) const
{
#ifdef USE_CUDA
    int width = image.width();
    int height = image.height();
    int pixel_count = width * height;

    QVector<float3> v_palette;
    v_palette.reserve(palette->size());

    for (int i = 0; i < palette->size(); ++i)
    {
        float paletter, paletteg, paletteb;

        palette->at(i).getRgbF(&paletter, &paletteg, &paletteb);

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

    // FIXME: I think this is wrong because we load images as
    // RGB not RGBA ..
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
