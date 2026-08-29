#ifndef REMAPPER_METAL_H
#define REMAPPER_METAL_H

#include <QColor>
#include <QImage>
#include <QList>

namespace MTL
{
class ComputePipelineState;
class Buffer;
}

class RemapperMetal
{
public:
    RemapperMetal(QImage &image, const QList<QColor> &palette);

    QImage remapEuclidean();
    QImage remapHue();
    QImage remapSaturation();
    QImage remapValue();

private:
    QImage remapInternal(MTL::ComputePipelineState *pipeline);

    QSize myResolution;
    std::size_t myPaletteLength;

    // Metal-specific
    MTL::ComputePipelineState *myRemapEuclideanPipeline;
    MTL::ComputePipelineState *myRemapHuePipeline;
    MTL::ComputePipelineState *myRemapSaturationPipeline;
    MTL::ComputePipelineState *myRemapValuePipeline;

    MTL::Buffer *myImage;
    MTL::Buffer *myPalette;
    MTL::Buffer *myImageOut;
};

#endif  // REMAPPER_METAL_H
