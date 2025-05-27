#ifndef QUANTIZER_H
#define QUANTIZER_H

#include <QColor>
#include <QImage>
#include <QVector>

class Quantizer
{
public:
    Quantizer(const int palette_size);
    ~Quantizer() = default;

    // Generates a palette for the given image. Uses median cut to produce the
    // palette
    QVector<QColor> generatePalette(const QImage &image) const;

private:
    int myPaletteSize;
};

#endif
