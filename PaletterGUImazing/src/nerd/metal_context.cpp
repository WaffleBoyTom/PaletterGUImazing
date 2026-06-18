#include "metal_context.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QString>

MetalContext::MetalContext()
{
    const QString theExecutablePath = QCoreApplication::applicationDirPath();
    const QString theMetalShaderLibraryPath =
        theExecutablePath + "/shaders.metallib";

    NS::Error *error;

    myPool = NS::AutoreleasePool::alloc()->init();
    myDevice = MTL::CreateSystemDefaultDevice();
    if (myDevice == nullptr)
    {
        qDebug() << "Metal error: failed to create device.";
        qApp->quit();
    }

    myCommandQueue = myDevice->newCommandQueue();
    if (myCommandQueue == nullptr)
    {
        qDebug() << "Metal error: failed to create command queue.";
        qApp->quit();
    }

    qDebug() << "Looking for metallib at: " << theMetalShaderLibraryPath;
    if (!QFile::exists(theMetalShaderLibraryPath))
    {
        qDebug() << "Failed to find metallib file.";
        qApp->quit();
    }

    NS::URL *url = NS::URL::fileURLWithPath(
        NS::String::string(
            theMetalShaderLibraryPath.toUtf8().constData(),
            NS::ASCIIStringEncoding
        )
    );

    myLibrary = myDevice->newLibrary(url, &error);
    if (myLibrary == nullptr)
    {
        qDebug() << "Metal error: failed to load library.";
        qApp->quit();
    }
    else if (error != nullptr)
    {
        qDebug() << QString("Metal error: %1")
                        .arg(error->localizedDescription()->cString(
                            NS::ASCIIStringEncoding
                        ));
        qApp->quit();
    }
}

MTL::ComputePipelineState *
MetalContext::createPipelineState(const char *library_name)
{
    NS::Error *error;

    NS::String *function_name =
        NS::String::string(library_name, NS::ASCIIStringEncoding);
    MTL::Function *function = myLibrary->newFunction(function_name);
    if (function == nullptr)
    {
        qDebug() << "Metal error: failed to create function.";
        qApp->quit();
    }

    MTL::ComputePipelineState *compute_pipeline_state =
        myDevice->newComputePipelineState(function, &error);
    if (compute_pipeline_state == nullptr)
    {
        qDebug() << "Metal error: failed to create compute pipeline state.";
        qApp->quit();
    }
    else if (error != nullptr)
    {
        qDebug() << "Metal error: "
                 << error->localizedDescription()->cString(
                        NS::ASCIIStringEncoding
                    );
        qApp->quit();
    }

    return compute_pipeline_state;
}

MTL::Buffer *
MetalContext::createSharedBuffer(std::size_t size)
{
    MTL::Buffer *buffer =
        myDevice->newBuffer(size, MTL::ResourceStorageModeShared);
    if (buffer == nullptr)
    {
        qDebug() << "Metal error: failed to create buffer.";
        qApp->quit();
    }

    return buffer;
}

MTL::CommandBuffer *
MetalContext::createCommandBuffer()
{
    MTL::CommandBuffer *command_buffer = myCommandQueue->commandBuffer();
    if (command_buffer == nullptr)
    {
        qDebug() << "Metal error: failed to create command buffer.";
        qApp->quit();
    }

    return command_buffer;
}

MetalContext::~MetalContext()
{
    myPool->release();
}
