#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <QString>
#include <QJsonArray>
class FileHandler
{
public:

    static bool saveToFile(const QString &path, const QJsonObject &rootObj);
    static QJsonObject loadFromFile(const QString &path);
};

#endif // FILEHANDLER_H
