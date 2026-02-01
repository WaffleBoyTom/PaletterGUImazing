#include "zoom.cuh"
/// including this header causes nvcc to ragequit !!!
// #include <QDebug>


inline __device__
float3 
uchar4ToFloat3(uchar4 pixel)
{
    return make_float3(pixel.x / 255.0, 
                       pixel.y / 255.0,
                       pixel.z / 255.0);
}

inline __device__ 
uchar4 
float3ToUChar4(float3 color) {
    return make_uchar4(
        (unsigned char)(color.x * 255.0f),
        (unsigned char)(color.y * 255.0f),
        (unsigned char)(color.z * 255.0f),
        255 
    );
}


inline __device__ 
float 
euclidDist(float3 a, float3 b)
{ 
    float rdiff = b.x - a.x;
    float gdiff = b.y - a.y;
    float bdiff = b.z - a.z;

    return sqrtf(rdiff * rdiff + gdiff * gdiff + bdiff * bdiff);
}

__global__ void
test_kernel(int n, float *x, float *y) 
{  
    for (int i = 0; i < n; ++i)
        y[i] = x[i] + y[i];
}

__global__ 
void
applyPaletteByLengthKernel(uchar4 *pixels, 
                           const int pixel_count,
                           const float3 *palette,
                           const int palette_size)
{    
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;

    for (int i = index; i < pixel_count; i += stride)
    {
        float delta = 1000.0;
        uchar4 bits = pixels[i];
        float3 pixel = uchar4ToFloat3(bits);
        float3 best = pixel;

        for (int j = 0; j < palette_size; ++j)
        {
            float3 palette_col = palette[j];
            float dist = euclidDist(pixel, palette_col);
            if (dist < delta)
            {
                best = palette_col;
                delta = dist;
            }
            if (dist < 0.01)
                break;
        }
        pixels[i] = float3ToUChar4(best);
    }   

}

namespace Zoom 
{


void test(const int n, float *x, float *y)
{
    
    test_kernel<<<1, 1>>>(n, x, y);

    cudaDeviceSynchronize();

}

void applyPaletteByLength(uchar4 *img, const int width, const int height, 
                          const float3 *palette,
                          const int palette_size)
{  
    const int pixel_count = width * height;
    // somewhat arbitrary block size 
    int block_size = 256;
    int blocks = (pixel_count + block_size - 1) / block_size;

    applyPaletteByLengthKernel<<<blocks, block_size>>>(
        img, pixel_count, 
        palette, palette_size
    );
    
    cudaDeviceSynchronize();
}
    

} //end namespace
