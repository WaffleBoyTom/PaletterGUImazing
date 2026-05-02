#include "zoom.cuh"
/// including this header causes nvcc to ragequit !!!
// #include <QDebug>

/// CUDA functions

inline __device__ float3
uchar4ToFloat3(uchar4 pixel)
{
    return make_float3(pixel.x / 255.0f, pixel.y / 255.0f, pixel.z / 255.0f);
}

inline __device__ float3
uchar3ToFloat3(uchar3 pixel)
{
    return make_float3(pixel.x / 255.0f, pixel.y / 255.0f, pixel.z / 255.0f);
}

inline __device__ uchar4
float3ToUChar4(float3 color)
{
    // red and green need to be swapped here
    // seems sus but something to do with QFormat of the QImage
    // I believe
    return make_uchar4(
        (unsigned char)(color.z * 255.0f),
        (unsigned char)(color.y * 255.0f),
        (unsigned char)(color.x * 255.0f),
        255
    );
}

inline __device__ uchar3
float3ToUChar3(float3 color)
{
    return make_uchar3(
        (unsigned char)(color.x * 255.0f),
        (unsigned char)(color.y * 255.0f),
        (unsigned char)(color.z * 255.0f)
    );
}

inline __device__ float
euclidDist(float3 a, float3 b)
{
    float rdiff = b.x - a.x;
    float gdiff = b.y - a.y;
    float bdiff = b.z - a.z;

    return sqrtf(rdiff * rdiff + gdiff * gdiff + bdiff * bdiff);
}

inline __device__ float
euclidDist2(float3 a, float3 b)
{
    float rdiff = b.x - a.x;
    float gdiff = b.y - a.y;
    float bdiff = b.z - a.z;

    return rdiff * rdiff + gdiff * gdiff + bdiff * bdiff;
}
inline __device__ bool
isEqual(float a, float b)
{
    return fabs(a - b) < 1e-6;
}

inline __device__ float3
rgbToHsv(float3 rgb)
{
    /*
    https://math.stackexchange.com/questions/556341/rgb-to-hsv-color-conversion-algorithm
    algo pulled from here
    there might be a better way to do the conversion :shrug:
    */
    float3 hsv = make_float3(0.0, 0.0, 0.0);
    float cmax = fmaxf(rgb.x, fmaxf(rgb.y, rgb.z));
    float cmin = fminf(rgb.x, fminf(rgb.y, rgb.z));
    float d = cmax - cmin;

    float value = cmax;
    float sat = cmax == 0.0 ? 0.0 : d / cmax;
    float hue = 0.0;

    hsv.x = hue;
    hsv.y = sat;
    hsv.z = value;

    // if delta is 0.0 then hue is 0 and we return early
    // otherwise we would divide by 0.0
    if (d == 0)
        return hsv;

    // hue is more annoying to compute
    if (isEqual(cmax, rgb.x))
    {
        hsv.x = fmodf((rgb.y - rgb.z) / d, 6.0);
    }
    else if (isEqual(cmax, rgb.y))
    {
        hsv.x = (rgb.z - rgb.x) / d + 2.0;
    }
    else if (isEqual(cmax, rgb.z))
    {
        hsv.x = (rgb.x - rgb.y) / d + 4.0;
    }
    // to get a hue from 0.0 -> 360 we should multiply by
    // 60.0 as we fmod(, 6.0)
    // but we dgaf because we just use this for comparison
    // so we don't actually need the usual range..
    // which means we end up with a range from 0-6 for hue
    // and 0-1 for everything else
    // normalize
    hsv.x = hsv.x * 60. / 360.;

    return hsv;
}

// FIXME: do something less stupid to avoid duplicate code
// TODO: quite a bit of duplicate code here ...

__global__ void
applyPaletteByLengthKernel(
    uchar4 *pixels,
    const int pixel_count,
    const float3 *palette,
    const int palette_size
)
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
            float dist = euclidDist2(pixel, palette_col);
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

__global__ void
applyPaletteByLuminanceKernel(
    uchar4 *pixels,
    const int pixel_count,
    const float3 *palette,
    const int palette_size
)
{
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;

    for (int i = index; i < pixel_count; i += stride)
    {
        float delta = 1000.0;
        uchar4 bits = pixels[i];
        float3 pixel = uchar4ToFloat3(bits);
        float3 best = pixel;
        pixel = rgbToHsv(pixel);

        for (int j = 0; j < palette_size; ++j)
        {
            float3 palette_col = palette[j];
            float3 palette_hsv = rgbToHsv(palette_col);
            float ldelta = abs(pixel.z - palette_hsv.z);
            if (ldelta < delta)
            {
                best = palette_col;
                delta = ldelta;
            }
            if (ldelta < 0.01)
                break;
        }
        pixels[i] = float3ToUChar4(best);
    }
}

__global__ void
applyPaletteByHueKernel(
    uchar4 *pixels,
    const int pixel_count,
    const float3 *palette,
    const int palette_size
)
{
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;

    for (int i = index; i < pixel_count; i += stride)
    {
        float delta = 1000.0;
        uchar4 bits = pixels[i];
        float3 pixel = uchar4ToFloat3(bits);
        float3 best = pixel;
        pixel = rgbToHsv(pixel);

        for (int j = 0; j < palette_size; ++j)
        {
            float3 palette_col = palette[j];
            float3 palette_hsv = rgbToHsv(palette_col);
            float ldelta = fminf(
                abs(pixel.x - palette_hsv.x),
                abs((pixel.x + 1.0) - palette_hsv.x)
            );
            if (ldelta < delta)
            {
                best = palette_col;
                delta = ldelta;
            }
            // if (ldelta < 0.01)
            //     break;
        }
        pixels[i] = float3ToUChar4(best);
    }
}
__global__ void
applyPaletteBySaturationKernel(
    uchar4 *pixels,
    const int pixel_count,
    const float3 *palette,
    const int palette_size
)
{
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;

    for (int i = index; i < pixel_count; i += stride)
    {
        float delta = 1000.0;
        uchar4 bits = pixels[i];
        float3 pixel = uchar4ToFloat3(bits);
        float3 best = pixel;
        pixel = rgbToHsv(pixel);

        for (int j = 0; j < palette_size; ++j)
        {
            float3 palette_col = palette[j];
            float3 palette_hsv = rgbToHsv(palette_col);
            float ldelta = abs(pixel.y - palette_hsv.y);
            if (ldelta < delta)
            {
                best = palette_col;
                delta = ldelta;
            }
            if (ldelta < 0.01)
                break;
        }
        pixels[i] = float3ToUChar4(best);
    }
}

/// end of CUDA code
/// C++ functions

namespace Zoom
{
void
applyPaletteByLength(
    uchar4 *img,
    const int width,
    const int height,
    const float3 *palette,
    const int palette_size
)
{
    const int pixel_count = width * height;
    // somewhat arbitrary block size
    int block_size = 256;
    int blocks = (pixel_count + block_size - 1) / block_size;

    applyPaletteByLengthKernel<<<blocks, block_size>>>(
        img, pixel_count, palette, palette_size
    );

    cudaDeviceSynchronize();
}

void
applyPaletteByLuminance(
    uchar4 *img,
    const int width,
    const int height,
    const float3 *palette,
    const int palette_size
)
{
    const int pixel_count = width * height;
    // somewhat arbitrary block size
    int block_size = 256;
    int blocks = (pixel_count + block_size - 1) / block_size;

    applyPaletteByLuminanceKernel<<<blocks, block_size>>>(
        img, pixel_count, palette, palette_size
    );

    cudaDeviceSynchronize();
}
void
applyPaletteByHue(
    uchar4 *img,
    const int width,
    const int height,
    const float3 *palette,
    const int palette_size
)
{
    const int pixel_count = width * height;
    // somewhat arbitrary block size
    int block_size = 256;
    int blocks = (pixel_count + block_size - 1) / block_size;

    applyPaletteByHueKernel<<<blocks, block_size>>>(
        img, pixel_count, palette, palette_size
    );

    cudaDeviceSynchronize();
}
void
applyPaletteBySaturation(
    uchar4 *img,
    const int width,
    const int height,
    const float3 *palette,
    const int palette_size
)
{
    const int pixel_count = width * height;
    // somewhat arbitrary block size
    int block_size = 256;
    int blocks = (pixel_count + block_size - 1) / block_size;

    applyPaletteBySaturationKernel<<<blocks, block_size>>>(
        img, pixel_count, palette, palette_size
    );

    cudaDeviceSynchronize();
}
}  // end namespace
