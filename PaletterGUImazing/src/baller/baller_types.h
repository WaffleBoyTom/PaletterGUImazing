#ifndef BALLER_TYPES_H
#define BALLER_TYPES_H

#include <QtCore/qassert.h>

enum class BallerDevice
{
    CPU = 0,
    GPU,
};

inline const char *
getDeviceStr(BallerDevice device)
{
    switch (device)
    {
    case BallerDevice::CPU:
        return "CPU";
    case BallerDevice::GPU:
        return "GPU";
    }
    Q_ASSERT("How did we get here !!");

    return "CPU";
}

#endif  // BALLER_TYPES_H
