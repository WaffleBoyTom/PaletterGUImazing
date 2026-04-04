#ifndef KMEANIFIER_H
#define KMEANIFIER_H

#include <QColor>
#include <QImage>
#include <QVector>

class KMeanifier
{
public:

    KMeanifier(int palette_size);

    // does the kmeans to generate palette
    // trust me bro guarantee
    QVector<QColor> generatePalette(const QImage &image) const;

private:
    int myPaletteSize;
};

#endif // KMEANIFIER_H
