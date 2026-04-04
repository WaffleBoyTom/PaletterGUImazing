#include "cumeans.cuh"
#include <cuda_runtime.h>

namespace CuMeans
{
void 
palettize(
    float3 *palette,
    const uchar4 *img,
    const int width,
    const int height,
    const int palette_size  
)
{
    
    const int pixel_count = width * height;
    // somewhat arbitrary block size
    int block_size = 256;
    int blocks = (pixel_count + block_size - 1) / block_size;

    cudaDeviceSynchronize();
}
} // end namespace
