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


}// end namespace


#endif // PALETTERUTILS_H
