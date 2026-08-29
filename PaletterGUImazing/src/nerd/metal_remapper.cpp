#include "metal_remapper.h"

#include <QApplication>
#include <QDebug>
#include <QDir>

#include "metal_context.h"
#include "metal_dispatcher.h"

RemapperMetal::RemapperMetal(QImage &image, const QList<QColor> &palette)
    : myResolution(image.size()), myPaletteLength(palette.length())
{
    MetalContext &context = MetalContext::instance();

    myRemapEuclideanPipeline = context.createPipelineState("remapEuclidean");
    myRemapHuePipeline = context.createPipelineState("remapHue");
    myRemapSaturationPipeline = context.createPipelineState("remapSaturation");
    myRemapValuePipeline = context.createPipelineState("remapValue");

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

    // Create and load the palette buffer.
    const std::size_t palette_size = palette.length() * sizeof(QRgb);
    myPalette = context.createSharedBuffer(palette_size);

    QRgb *palette_ptr = reinterpret_cast<QRgb *>(myPalette->contents());
    Q_ASSERT(palette_ptr != nullptr);

    for (std::size_t i = 0; i < palette.length(); ++i)
    {
        QColor color = palette[i];
        QRgb rgb = color.rgb();
        palette_ptr[i] = rgb;
    }

    myImageOut = context.createSharedBuffer(image_size);
}

QImage
RemapperMetal::remapEuclidean()
{
    return remapInternal(myRemapEuclideanPipeline);
}

QImage
RemapperMetal::remapHue()
{
    return remapInternal(myRemapHuePipeline);
}

QImage
RemapperMetal::remapSaturation()
{
    return remapInternal(myRemapSaturationPipeline);
}

QImage
RemapperMetal::remapValue()
{
    return remapInternal(myRemapValuePipeline);
}

QImage
RemapperMetal::remapInternal(MTL::ComputePipelineState *pipeline)
{
    MetalContext &context = MetalContext::instance();

    MetalDispatcher dispatcher(pipeline);

    dispatcher.bindBuffer(myImage, 0, 0);
    dispatcher.bindBuffer(myPalette, 0, 1);
    dispatcher.bindBytes(&myPaletteLength, sizeof(myPaletteLength), 2);
    dispatcher.bindBuffer(myImageOut, 0, 3);

    const std::size_t pixel_count =
        myResolution.width() * myResolution.height();
    const MTL::Size grid_size(pixel_count, 1, 1);
    dispatcher.dispatch(grid_size);

    QRgb *image_out_ptr = reinterpret_cast<QRgb *>(myImageOut->contents());
    Q_ASSERT(image_out_ptr != nullptr);

    QImage out_image(myResolution, QImage::Format_RGB32);
    for (std::size_t y = 0, height = myResolution.height(); y < height; ++y)
    {
        for (std::size_t x = 0, width = myResolution.width(); x < width; ++x)
        {
            const QRgb rgb = image_out_ptr[y * width + x];
            out_image.setPixel(x, y, rgb);
        }
    }

    return out_image;
}
