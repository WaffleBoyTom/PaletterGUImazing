/// CUDA Utility Functions

namespace CUtils
{

inline __device__ float3 uchar4ToFloat3(uchar4 pixel)
{    
    return make_float3(pixel.x / 255.0f, pixel.y / 255.0f, pixel.z / 255.0f);
}


inline __device__ bool isEqual(float a, float b)
{    
    return fabs(a - b) < 1e-6;
}

// euclidean distance
inline __device__ float dist(float3 a, float3 b)
{    
    float rdiff = b.x - a.x;
    float gdiff = b.y - a.y;
    float bdiff = b.z - a.z;

    return sqrtf(rdiff * rdiff + gdiff * gdiff + bdiff * bdiff);
}

// euclidean squared distance (no sqrt)
inline __device__ float dist2(float3 a, float3 b)
{
    
    float rdiff = b.x - a.x;
    float gdiff = b.y - a.y;
    float bdiff = b.z - a.z;

    return rdiff * rdiff + gdiff * gdiff + bdiff * bdiff;
}

}
