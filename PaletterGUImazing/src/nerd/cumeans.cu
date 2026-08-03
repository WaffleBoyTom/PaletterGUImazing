
#include "cumeans.cuh"
#include "cutils.cuh"
// for CUDART_MAX_NORMAL_F
#include <math_constants.h>


__global__ void 
assignClustersKernel(
    const uchar4* image, 
    const float3* palette, int* assignments,
    int width, int height, int k)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    
    if (x >= width || y >= height) 
        return;
    
    int idx = y * width + x;
    uchar4 pixel = image[idx];

    float3 rgb = CUtils::uchar4ToFloat3(pixel);
    
    float min_dist = CUDART_MAX_NORMAL_F;
    int best_cluster = 0;
    
    for (int i = 0; i < k; ++i) 
    {
        float3 c = palette[i];
        float dist_sq = CUtils::dist2(rgb, c);
        
        if (dist_sq < min_dist) 
        {
            min_dist = dist_sq;
            best_cluster = i;
        }
    }
    
    assignments[idx] = best_cluster;
    
}

__global__ void 
accumulateClustersKernel(
    const uchar4* image, 
    const int* assignments, 
    float3* sums, 
    int* counts, 
    int width, int height)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    
    if (x >= width || y >= height) 
        return;
    
    int idx = y * width + x;
    int cluster = assignments[idx];
    uchar4 pixel = image[idx];
    
    float3 rgb = CUtils::uchar4ToFloat3(pixel);
    
    atomicAdd(&(sums[cluster].x), rgb.x);
    atomicAdd(&(sums[cluster].y), rgb.y);
    atomicAdd(&(sums[cluster].z), rgb.z);
    atomicAdd(&(counts[cluster]), 1);

}

__global__ void 
updateCentroidsKernel(
    float3* palette, 
    const float3* sums, 
    const int* counts, 
    int palette_size) 
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < palette_size) 
    {
        int count = counts[idx];

        if (count > 0) 
        {
            palette[idx].x = sums[idx].x / count;
            palette[idx].y = sums[idx].y / count;
            palette[idx].z = sums[idx].z / count;
        }
    }

}

namespace CuMeans
{
void
assignClusters(
    const uchar4* image, 
    const float3* palette, int* assignments,
    int width, int height, int palette_size)
{
    
    dim3 block(16, 16);
    dim3 grid((width + block.x - 1) / block.x, 
              (height + block.y - 1) / block.y);

    assignClustersKernel<<<grid, block>>>(
        image, palette, assignments, 
        width, height, palette_size
    );
    
}

void 
accumulateClusters(
    const uchar4* image, 
    const int* assignments, 
    float3* sums, 
    int* counts, 
    int width, int height)
{
    
    dim3 block(16, 16);
    dim3 grid((width + block.x - 1) / block.x, 
              (height + block.y - 1) / block.y);
    
    accumulateClustersKernel<<<grid, block>>>(
        image, assignments, sums, counts, width, height);
    
}

void 
updateCentroids(
    float3* palette, 
    const float3* sums, 
    const int* counts, 
    int palette_size) 
{
    int threads = 256;
    int blocks = (palette_size + threads - 1) / threads;
    updateCentroidsKernel<<<blocks, threads>>>(
        palette, sums, counts, palette_size);
}


}  // end namespace
