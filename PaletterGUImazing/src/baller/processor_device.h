#ifndef PROCESSOR_DEVICE_H
#define PROCESSOR_DEVICE_H

#include <QtCore/qassert.h>

enum class PaletteProcessorDevice
{
    CPU = 0,
    GPU,
};

inline const char *
getDeviceStr(PaletteProcessorDevice device)
{
    switch (device)
    {
    case PaletteProcessorDevice::CPU:
        return "CPU";
    case PaletteProcessorDevice::GPU:
        return "GPU";
    }
    Q_ASSERT("How did we get here !!");

    return "CPU";
}

#endif  // PROCESSOR_DEVICE_H
