#include "zoom.cuh"
/// including this header causes nvcc to ragequit !!!
// #include <QDebug>
#include <stdio.h>

__global__ void
test_kernel(int n, float *x, float *y) 
{  
    for (int i = 0; i < n; ++i)
        y[i] = x[i] + y[i];
}

namespace Zoom 
{
	void test(const int n, float *x, float *y)
	{
        
        test_kernel<<<1, 1>>>(n, x, y);

        // Wait for GPU to finish before accessing on host
        cudaDeviceSynchronize();

    }
} //end namespace
