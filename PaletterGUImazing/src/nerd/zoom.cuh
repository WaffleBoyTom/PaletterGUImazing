#pragma once
#include <cuda_runtime.h>

namespace Zoom
{

void test(const int n, float *x, float *y);

void applyPaletteByLength(
    uchar4 *img,
    const int width,
    const int height,
    const float3 *palette,
    const int palette_size
);

}  // end namespace
