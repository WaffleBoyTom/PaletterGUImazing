#pragma once
#include <cuda_runtime.h>

namespace Zoom
{

void applyPaletteByLength(
    uchar4 *img,
    const int width,
    const int height,
    const float3 *palette,
    const int palette_size
);

void applyPaletteByLuminance(
    uchar4 *img,
    const int width,
    const int height,
    const float3 *palette,
    const int palette_size
);

void applyPaletteByHue(
    uchar4 *img,
    const int width,
    const int height,
    const float3 *palette,
    const int palette_size
);

void applyPaletteBySaturation(
    uchar4 *img,
    const int width,
    const int height,
    const float3 *palette,
    const int palette_size
);



}  // end namespace
