#ifndef SICK_UTILS_H
#define SICK_UTILS_H

#include <QString>

namespace SickUtils
{

/// returns a QString of the supported file formats
/// by the QImageReader
/// to be used with a QFileDialog
/// maybe this should live elsewhere ?
QString supportedImageFormats();
/// returns a QString of the support export file formats
/// for the palette which is just JSON right now  
QString supportedPaletteFormats();


} // end namespace

#endif // SICK_UTILS_H
