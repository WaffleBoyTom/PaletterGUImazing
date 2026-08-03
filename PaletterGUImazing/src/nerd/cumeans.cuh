#pragma once
#include <cuda_runtime.h>

namespace CuMeans
{
void assignClusters(
    const uchar4* image, 
    const float3* palette, int* assignments,
    int width, int height, int palette_size
);

void accumulateClusters(
    const uchar4* image, 
    const int* assignments, 
    float3* sums, 
    int* counts, 
    int width, int height
);

void updateCentroids(
    float3* palette, 
    const float3* sums, 
    const int* counts, int palette_size
);

}  // end namespace
