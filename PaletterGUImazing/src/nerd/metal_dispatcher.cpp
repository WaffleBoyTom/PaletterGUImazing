#include "metal_dispatcher.h"

#include <QApplication>
#include <QDebug>

#include "Foundation/NSTypes.hpp"
#include "metal_context.h"

MetalDispatcher::MetalDispatcher(MTL::ComputePipelineState *pipeline)
{
    MetalContext &context = MetalContext::instance();

    myCommandBuffer = context.createCommandBuffer();

    myCommandEncoder = myCommandBuffer->computeCommandEncoder();
    if (myCommandEncoder == nullptr)
    {
        qDebug() << "Metal error: failed to retrieve compute command encoder.";
        qApp->quit();
    }

    myPipelineState = pipeline;
    myCommandEncoder->setComputePipelineState(myPipelineState);
}

void
MetalDispatcher::bindBuffer(
    MTL::Buffer *buffer, std::size_t offset, std::size_t index
)
{
    myCommandEncoder->setBuffer(
        buffer,
        static_cast<NS::UInteger>(offset),
        static_cast<NS::UInteger>(index)
    );
}

void
MetalDispatcher::bindBytes(
    const void *data, std::size_t length, std::size_t index
)
{
    myCommandEncoder->setBytes(
        data,
        static_cast<NS::UInteger>(length),
        static_cast<NS::UInteger>(index)
    );
}

void
MetalDispatcher::dispatch(const MTL::Size grid_size)
{
    const NS::UInteger grid_volume =
        grid_size.width * grid_size.height * grid_size.depth;

    NS::UInteger thread_group_count =
        myPipelineState->maxTotalThreadsPerThreadgroup();
    if (thread_group_count > grid_volume)
        thread_group_count = grid_volume;

    qDebug() << "Using thread group count "
             << static_cast<std::size_t>(thread_group_count);

    const MTL::Size thread_group_size(thread_group_count, 1, 1);

    myCommandEncoder->dispatchThreads(grid_size, thread_group_size);
    myCommandEncoder->endEncoding();

    myCommandBuffer->commit();
    myCommandBuffer->waitUntilCompleted();
}
