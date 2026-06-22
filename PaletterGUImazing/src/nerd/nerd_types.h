#ifndef NERD_TYPES_H
#define NERD_TYPES_H

enum class NerdPaletteAlgorithm
{
    MedianCut = 0,
    KMeans
};

enum class NerdCompareMethod
{
    Distance = 0,
    Luminance,
    Hue,
    Saturation
};

#endif // NERD_TYPES_H
