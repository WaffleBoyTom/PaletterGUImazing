#ifndef REMAPPER_H
#define REMAPPER_H

#include <QColor>
#include <QImage>
#include <QList>
#include <QPixmap>

enum class CompareMethod
{
    Distance = 0,
    Luminance,
    Hue,
    Saturation
};

class Remapper
{
public:
    Remapper(CompareMethod method, const QList<QColor> &palette);

    void remap(QImage &image) const;
    void remapHost(QImage &image) const;

private:
    void remapCuda(QImage &image) const;
    void remapMetal(QImage &image) const;

    CompareMethod myCompareMethod;
    const QList<QColor> &myPalette;
};

#endif  // REMAPPER_H
