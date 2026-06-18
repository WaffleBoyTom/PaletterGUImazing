#ifndef REMAPPER_METAL_H
#define REMAPPER_METAL_H

#include <QColor>
#include <QImage>
#include <QList>

#include "metal_context.h"

namespace MTL
{
class ComputePipelineState;
class Buffer;
}

class RemapperMetal
{
public:
    RemapperMetal(QImage &image, const QList<QColor> &palette);

    QImage quantizeEuclidean();
    QImage quantizeHue();
    QImage quantizeSaturation();
    QImage quantizeValue();

private:
    QImage quantizeInternal(MTL::ComputePipelineState *pipeline);

    QSize myResolution;
    std::size_t myPaletteLength;

    // Metal-specific
    MetalContext myContext;

    MTL::ComputePipelineState *myQuantizeEuclideanPipeline;
    MTL::ComputePipelineState *myQuantizeHuePipeline;
    MTL::ComputePipelineState *myQuantizeSaturationPipeline;
    MTL::ComputePipelineState *myQuantizeValuePipeline;

    MTL::Buffer *myImage;
    MTL::Buffer *myPalette;
    MTL::Buffer *myImageOut;
};

#endif  // REMAPPER_METAL_H
