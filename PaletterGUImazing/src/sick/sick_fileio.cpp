#include "sick_fileio.h"

SickJsonIO::SickJsonIO(const QJsonObject *json, const QString *path)
    : myJson(json), myPath(path)
{
}

bool
SickJsonIO::openFile(QFile &file)
{
    // I don't trust these flags, idk what they do
    // TODO: look into these sus flags
    return file.open(
        QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate
    );
}

bool
SickJsonIO::write(QString &err)
{
    if (!myJson)
    {
        err = "JSon object is invalid !";
        return false;
    }
    if (!myPath)
    {
        err = "Path is invalid !";
        return false;
    }
    // TODO: make sure we always export to json
    //  cuz user might type something like ballllls.jpg
    //  this is not enough because user could type
    //  fuck it up royally but hey, whatever
    if (!myPath->endsWith(".json"))
    {
        err =
            "File name should end in .json.\n"
            "By default we append the .json extension for you !";
        return false;
    }

    QFile out_file(*myPath);
    if (!openFile(out_file))
    {
        err = "Something went wrong with that file !!!!!";
        return false;
    }

    QJsonDocument doc(*myJson);
    QByteArray json_data = doc.toJson(QJsonDocument::Indented);

    out_file.write(json_data);
    out_file.close();

    err = QString("Written palette to %1").arg(*myPath);
    return true;
}
