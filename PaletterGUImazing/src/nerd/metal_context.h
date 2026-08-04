#ifndef METAL_CONTEXT_H
#define METAL_CONTEXT_H

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

// Source: https://github.com/postmalloc/metal-cpp-example

class MetalContext
{
public:
    // TIL about Meyers Singleton (!)
    static MetalContext &instance();

    MetalContext(MetalContext &other) = delete;
    MetalContext &operator=(MetalContext &other) = delete;

    MTL::ComputePipelineState *createPipelineState(const char *library_name);
    MTL::Buffer *createSharedBuffer(std::size_t size);
    MTL::CommandBuffer *createCommandBuffer();

protected:
    MetalContext();
    ~MetalContext();

private:
    // RAII object pool.
    NS::AutoreleasePool *myPool;

    // Physical GPU device.
    MTL::Device *myDevice;

    // Render command queue.
    MTL::CommandQueue *myCommandQueue;

    // Compiled metal shader functions
    MTL::Library *myLibrary;
};

#endif  // METAL_CONTEXT_H
