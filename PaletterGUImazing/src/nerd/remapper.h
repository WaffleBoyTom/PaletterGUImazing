#ifndef REMAPPER_H
#define REMAPPER_H

#include <QColor>
#include <QImage>
#include <QList>
#include <QPixmap>

class Remapper
{
public:
    enum class Colorspace
    {
        Distance = 0,
        Luminance,
        Hue,
        Saturation
    };

    Remapper(Colorspace colorspace, const QList<QColor> &palette);

    void remap(QImage &image) const;
    void remapHost(QImage &image) const;

private:
    [[gnu::unused]] Colorspace myColorspace;
    const QList<QColor> &myPalette;
};

#endif
