#ifndef SICK_FILEIO_H
#define SICK_FILEIO_H

#include <QtWidgets>

// NOTE: should this just have static methods instead ?
class CoolUtilJsonIO
{
public:
    explicit CoolUtilJsonIO(const QJsonObject *json, const QString *path);

    // if returns false, operation failed,
    // error message is written to err
    bool write(QString &err);

private:
    bool openFile(QFile &file);
    const QJsonObject *myJson;
    const QString *myPath;
};

// SickImageIO ??

#endif
