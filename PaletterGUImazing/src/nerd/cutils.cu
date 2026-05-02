#include "cutils.cuh"

namespace CUtils
{

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

inline __device__ bool
isEqual(float a, float b)
{
    return fabs(a - b) < 1e-6;
}

}  // end namespace
