#include "weatherfetcher.h"
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>
#include <QDebug>

WeatherFetcher::WeatherFetcher(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &WeatherFetcher::onReplyFinished); // 暂时无用，可支持异步扩展
}

QString iconForDescription(const QString &desc) {
    if (desc.contains("晴")) return "☀️";
    if (desc.contains("多云")) return "⛅";
    if (desc.contains("阴")) return "☁️";
    if (desc.contains("雨")) return "🌧️";
    if (desc.contains("雷")) return "⛈️";
    if (desc.contains("雪")) return "❄️";
    if (desc.contains("雾")) return "🌫️";
    // 其他你可以自己加
    return "❓";
}

QString WeatherFetcher::getWeatherString(const QString &location)
{
    QString url = QString("https://wttr.in/%1?format=j1&lang=zh").arg(location);
    QUrl realUrl(url);
    QNetworkRequest request(realUrl);
    request.setRawHeader("User-Agent", "Mozilla/5.0");

    QNetworkReply *reply = manager->get(request);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        QString error = reply->errorString();
        reply->deleteLater();
        return "获取失败：" + error;
    }

    QByteArray data = reply->readAll();
    reply->deleteLater();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return "数据解析失败";

    QJsonObject root = doc.object();

    QJsonObject current = root["current_condition"].toArray()[0].toObject();
    QString description = current["lang_zh"].toArray()[0].toObject()["value"].toString();

    QJsonObject today = root["weather"].toArray()[0].toObject();
    QString minTemp = today["mintempC"].toString();
    QString maxTemp = today["maxtempC"].toString();

    QString icon = iconForDescription(description);

    return QString("%1  天气：%2  温度：%3℃ - %4℃").arg(icon).arg(description).arg(minTemp).arg(maxTemp);
}


void WeatherFetcher::onReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();
    // 异步用不到，保留
}
