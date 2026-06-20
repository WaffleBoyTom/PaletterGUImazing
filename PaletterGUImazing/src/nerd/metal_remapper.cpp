#include "metal_remapper.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <cassert>

#include "metal_context.h"
#include "metal_dispatcher.h"

RemapperMetal::RemapperMetal(QImage &image, const QList<QColor> &palette) :
    myResolution(image.size()),
    myPaletteLength(palette.length()),
    myContext()
{
    myQuantizeEuclideanPipeline = myContext.createPipelineState("quantizeEuclidean");
    myQuantizeHuePipeline = myContext.createPipelineState("quantizeHue");
    myQuantizeSaturationPipeline = myContext.createPipelineState("quantizeSaturation");
    myQuantizeValuePipeline = myContext.createPipelineState("quantizeValue");

    // Create and load the image buffer.
    const std::size_t image_res = image.width() * image.height();
    const std::size_t image_size = image_res * sizeof(QRgb);
    myImage = myContext.createSharedBuffer(image_size);

    QRgb *image_ptr = reinterpret_cast<QRgb *>(myImage->contents());
    assert(image_ptr != nullptr);

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
    myPalette = myContext.createSharedBuffer(palette_size);

    QRgb *palette_ptr = reinterpret_cast<QRgb *>(myPalette->contents());
    assert(palette_ptr != nullptr);

    for (std::size_t i = 0; i < palette.length(); ++i)
    {
        QColor color = palette[i];
        QRgb rgb = color.rgb();
        palette_ptr[i] = rgb;
    }

    myImageOut = myContext.createSharedBuffer(image_size);
}

QImage
RemapperMetal::quantizeEuclidean()
{
    return quantizeInternal(myQuantizeEuclideanPipeline);
}

QImage
RemapperMetal::quantizeHue()
{
    return quantizeInternal(myQuantizeHuePipeline);
}

QImage
RemapperMetal::quantizeSaturation()
{
    return quantizeInternal(myQuantizeSaturationPipeline);
}

QImage
RemapperMetal::quantizeValue()
{
    return quantizeInternal(myQuantizeValuePipeline);
}

QImage
RemapperMetal::quantizeInternal(MTL::ComputePipelineState *pipeline)
{
    MetalDispatcher dispatcher(myContext, pipeline);

    dispatcher.bindBuffer(myImage, 0, 0);
    dispatcher.bindBuffer(myPalette, 0, 1);
    dispatcher.bindBytes(&myPaletteLength, sizeof(myPaletteLength), 2);
    dispatcher.bindBuffer(myImageOut, 0, 3);

    const std::size_t pixel_count = myResolution.width() * myResolution.height();
    const MTL::Size grid_size(pixel_count, 1, 1);
    dispatcher.dispatch(grid_size);

    QRgb *image_out_ptr = reinterpret_cast<QRgb *>(myImageOut->contents());
    assert(image_out_ptr != nullptr);

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
