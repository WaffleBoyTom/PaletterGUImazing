#ifndef PALETTERUTILS_H
#define PALETTERUTILS_H

// #include <QtWidgets>
namespace PaletterUtils
{

enum class PaletteApplyMode
{
    Distance = 0,
    Luminance,
    Hue,
    Saturation       
};

enum class PaletteGenerationMode
{
    MedianCut = 0,
    K_Means
};

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
        case PaletteProcessorDevice::CPU: return "CPU";
        case PaletteProcessorDevice::GPU: return "GPU";
        
    }
    Q_ASSERT("How did we get here !!");
    
    return "CPU";
}


}// end namespace


#endif // PALETTERUTILS_H
