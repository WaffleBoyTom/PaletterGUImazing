#ifndef REMAPPER_H
#define REMAPPER_H

#include <QColor>
#include <QImage>
#include <QList>
#include <QPixmap>

#include "nerd_types.h"

class Remapper
{
public:
    Remapper(NerdCompareMethod method, const QList<QColor> &palette);

    void remap(QImage &image) const;
    void remapHost(QImage &image) const;

private:
    void remapCuda(QImage &image) const;
    void remapMetal(QImage &image) const;

    [[maybe_unused]]
    NerdCompareMethod myCompareMethod;
    const QList<QColor> &myPalette;
};

#endif  // REMAPPER_H
