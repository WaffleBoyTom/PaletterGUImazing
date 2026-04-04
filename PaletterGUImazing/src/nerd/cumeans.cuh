#pragma once

namespace CuMeans
{
    void palettize(
        float3 *palette,
        const uchar4 *img,
        const int width,
        const int height,
        const int palette_size  
    );



} // end namespace
