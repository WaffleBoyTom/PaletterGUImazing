#ifndef QUANTIZER_H
#define QUANTIZER_H

#include <QColor>
#include <QImage>
#include <QVector>

class Quantizer
{
public:
    enum class Method
    {
        MedianCut = 0,
        K_Means
    };

    Quantizer(int palette_size, Method method);
    ~Quantizer() = default;

    // Generates a palette for the given image using median cut.
    //
    // Median cut creates a palette by first flattening the pixel colors
    // into a 1D array of (typically) RGB values. The palette is then
    // iteratively constructed, with each iteration corresponding to a new
    // palette color.
    //
    // At a high level: the 1D array is divided into buckets, where each bucket
    // covers a contiguous span of the input array. Then, we take the average
    // color of all the colors in each bucket.
    //
    // Each recursion roughly corresponds to the following steps:
    //
    //  1. On the first iteration, we simply have the 1D array of RGB values.
    //  Otherwise, we have the input array divided into buckets of colors.
    //
    //  2. Do the following for every bucket in the input: for each of the red,
    //  green, and blue channels, find the difference between the max and the
    //  min values. For instance, say we have the colors
    //      {[120, 100, 140], [255, 255, 255], [10, 20, 150], [0, 100, 200]}
    //  The maximum red value is 255, and the minimum is 0. Then the difference
    //  is 255. The difference for green is 235, and for blue is 55. Thus the
    //  greatest difference is 255 for the red channel.
    //
    //  3. Determine the bucket with the *greatest channel difference*. This
    //  indicates that this bucket has the most variability in color over all
    //  other buckets. It would make sense then that we should split this bucket
    //  into two smaller buckets which can more accurately capture the full
    //  range of colors. So that is what we do.
    //
    //  4. Sort this bucket's array *by the maximum channel*. So continuing with
    //  the example from step 3, after sorting the array becomes
    //      {[0, 100, 200], [10, 20, 150], [120, 100, 140], [255, 255, 255]}
    //
    //  5. Split the array in half (i.e., by the *median value*). Each subarray
    //  becomes its own, new bucket. Again, following the example:
    //      {{[0, 100, 200], [10, 20, 150]}, {[120, 100, 140], [255, 255, 255]}}
    //
    // 6. Repeat until there are as many buckets as the desired palette size.
    // Then take the average of all the colors in each bucket, and return the
    // list of averages as the palette.
    QVector<QColor> generatePalette(const QImage &image) const;

private:
    int myPaletteSize;
    Method myMethod;
};

#endif
