#ifndef SICK_FILEIO_H
#define SICK_FILEIO_H

#include <QtWidgets>
QT_BEGIN_NAMESPACE
class QJsonObject;
class QString;
class QFile;
QT_END_NAMESPACE

// NOTE: should this just have static methods instead ?
class SickJsonIO
{
public:
    explicit SickJsonIO(const QJsonObject *json, 
                        const QString *path);

    // if returns false, operation failed,
    // error message is written to err
    bool write(QString &err);

private:
    bool  openFile(QFile &file);
    const QJsonObject *myJson;
    const QString *myPath;

};

// SickImageIO ??

#endif
