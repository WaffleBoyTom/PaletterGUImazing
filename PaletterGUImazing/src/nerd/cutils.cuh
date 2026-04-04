/// CUDA Utility Functions

namespace CUtils
{
    
inline __device__ float3 uchar4ToFloat3(uchar4 pixel);

inline __device__ float3 uchar3ToFloat3(uchar3 pixel);

inline __device__ uchar4 float3ToUChar4(float3 color);

inline __device__ uchar3 float3ToUChar3(float3 color);

inline __device__ bool   isEqual(float a, float b);

}
