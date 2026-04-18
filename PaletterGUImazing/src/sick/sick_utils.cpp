#include "sick_utils.h"
#include <QList>
#include <QByteArray>
#include <QImageReader>


namespace SickUtils
{

QString 
supportedImageFormats()
{

    QList<QByteArray> image_formats = 
        QImageReader::supportedImageFormats();
    
    QStringList format_filters;
    
    for (const QByteArray &image_format : image_formats)
        format_filters << QString("*.%1").arg(image_format);

    format_filters.removeAll("*.gif");

    auto all_name_filter = QString("All Images (%1)")
                           .arg(format_filters.join(" "));

    return all_name_filter;

    
}

QString
supportedPaletteFormats()
{
    auto fmts = QString("JSON Files (*.json)");
    return fmts;
}
  
} // end namespace
