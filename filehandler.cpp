#include "filehandler.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

bool FileHandler::saveToFile(const QString &path, const QJsonObject &rootObj) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonDocument doc(rootObj);
    file.write(doc.toJson());
    file.close();
    return true;
}

QJsonObject FileHandler::loadFromFile(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return QJsonObject();
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (doc.isNull() || !doc.isObject()) {
        return QJsonObject();
    }
    return doc.object();

}
