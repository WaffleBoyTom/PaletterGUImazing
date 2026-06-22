#include <metal_stdlib>

using namespace metal;

float3 rgbUcharToFloat(const uchar3 rgb)
{
    return float3(rgb) / 255.0f;
}

uchar3 rgbFloatToUchar(const float3 rgb)
{
    return uchar3(clamp(rgb, 0.0f, 1.0f) * 255.0f);
}

// Ported from zoom.cu
float3 rgbToHsv(const float3 rgb)
{
    const float cmax = fmax(rgb.x, fmax(rgb.y, rgb.z));
    const float cmin = fmin(rgb.x, fmin(rgb.y, rgb.z));
    const float d = cmax - cmin;

    float3 hsv = float3(0.0);

    hsv.x = 0.0f;
    hsv.y = cmax == 0.0f ? 0.0f : d / cmax;
    hsv.z = cmax;

    if (d == 0.0f)
    {
        return hsv;
    }

    if (cmax == rgb.x)
    {
        hsv.x = fmod((rgb.y - rgb.z) / d, 6.0f);
    }
    else if (cmax == rgb.y)
    {
        hsv.x = (rgb.z - rgb.x) / d + 2.0f;
    }
    else if (cmax == rgb.z)
    {
        hsv.x = (rgb.x - rgb.y) / d + 4.0f;
    }

    hsv.x = hsv.x * 60.0f / 360.0f;

    return hsv;
}

float distanceEuclidean(const float3 rgb1, const float3 rgb2)
{
    const float3 rgb = rgb1 - rgb2;
    return dot(rgb, rgb);
}

float distanceHue(const float3 hsv1, const float3 hsv2)
{
    return abs(hsv1.x - hsv2.x);
}

float distanceSaturation(const float3 hsv1, const float3 hsv2)
{
    return abs(hsv1.y - hsv2.y);
}

float distanceValue(const float3 hsv1, const float3 hsv2)
{
    return abs(hsv1.z - hsv2.z);
}

kernel void remapEuclidean(
    device const uchar4* image,
    device const uchar4* palette,
    constant uint& paletteLength,
    device uchar4* result,
    uint index [[thread_position_in_grid]]
)
{
    const uchar4 color_in = image[index];
    const float3 rgb_in = rgbUcharToFloat(color_in.xyz);

    float min_dist = 3.0f;
    float3 rgb_out = rgb_in;

    for (uint i = 0; i < paletteLength; i++)
    {
        const float3 rgb_palette = rgbUcharToFloat(palette[i].xyz);
        const float dist = distanceEuclidean(rgb_in, rgb_palette);
        if (dist < min_dist)
        {
            min_dist = dist;
            rgb_out = rgb_palette;
        }
        if (dist < 0.01)
        {
            break;
        }
    }
    result[index] = uchar4(rgbFloatToUchar(rgb_out), color_in.w);
}

kernel void remapHue(
    device const uchar4* image,
    device const uchar4* palette,
    constant uint& paletteLength,
    device uchar4* result,
    uint index [[thread_position_in_grid]]
)
{
    const uchar4 color_in = image[index];
    const float3 rgb_in = rgbUcharToFloat(color_in.xyz);

    float min_dist = 3.0f;
    float3 rgb_out = rgb_in;

    const float3 hsv_in = rgbToHsv(rgb_in);

    for (uint i = 0; i < paletteLength; i++)
    {
        const float3 rgb_palette = rgbUcharToFloat(palette[i].xyz);
        const float3 hsv_palette = rgbToHsv(rgb_palette);
        const float dist = distanceHue(hsv_in, hsv_palette);
        if (dist < min_dist)
        {
            min_dist = dist;
            rgb_out = rgb_palette;
        }
        if (dist < 0.01)
        {
            break;
        }
    }
    result[index] = uchar4(rgbFloatToUchar(rgb_out), color_in.w);
}

kernel void remapSaturation(
    device const uchar4* image,
    device const uchar4* palette,
    constant uint& paletteLength,
    device uchar4* result,
    uint index [[thread_position_in_grid]]
)
{
    const uchar4 color_in = image[index];
    const float3 rgb_in = rgbUcharToFloat(color_in.xyz);

    float min_dist = 3.0f;
    float3 rgb_out = rgb_in;

    const float3 hsv_in = rgbToHsv(rgb_in);

    for (uint i = 0; i < paletteLength; i++)
    {
        const float3 rgb_palette = rgbUcharToFloat(palette[i].xyz);
        const float3 hsv_palette = rgbToHsv(rgb_palette);
        const float dist = distanceSaturation(hsv_in, hsv_palette);
        if (dist < min_dist)
        {
            min_dist = dist;
            rgb_out = rgb_palette;
        }
        if (dist < 0.01)
        {
            break;
        }
    }
    result[index] = uchar4(rgbFloatToUchar(rgb_out), color_in.w);
}

kernel void remapValue(
    device const uchar4* image,
    device const uchar4* palette,
    constant uint& paletteLength,
    device uchar4* result,
    uint index [[thread_position_in_grid]]
)
{
    const uchar4 color_in = image[index];
    const float3 rgb_in = rgbUcharToFloat(color_in.xyz);

    float min_dist = 3.0f;
    float3 rgb_out = rgb_in;

    const float3 hsv_in = rgbToHsv(rgb_in);

    for (uint i = 0; i < paletteLength; i++)
    {
        const float3 rgb_palette = rgbUcharToFloat(palette[i].xyz);
        const float3 hsv_palette = rgbToHsv(rgb_palette);
        const float dist = distanceValue(hsv_in, hsv_palette);
        if (dist < min_dist)
        {
            min_dist = dist;
            rgb_out = rgb_palette;
        }
        if (dist < 0.01)
        {
            break;
        }
    }
    result[index] = uchar4(rgbFloatToUchar(rgb_out), color_in.w);
}
