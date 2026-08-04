#ifndef METAL_DISPATCHER_H
#define METAL_DISPATCHER_H

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

namespace MTL
{
class ComputePipelineState;
class Buffer;
}

class MetalDispatcher
{
public:
    MetalDispatcher(MTL::ComputePipelineState *pipeline);

    void bindBuffer(MTL::Buffer *buffer, std::size_t offset, std::size_t index);
    void bindBytes(const void *data, std::size_t length, std::size_t index);

    void dispatch(const MTL::Size grid_size);

private:
    MTL::CommandBuffer *myCommandBuffer;
    MTL::ComputeCommandEncoder *myCommandEncoder;
    MTL::ComputePipelineState *myPipelineState;
};

#endif  // METAL_DISPATCHER_H
