#ifndef WEATHERFETCHER_H
#define WEATHERFETCHER_H

#include <QObject>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

class WeatherFetcher : public QObject
{
    Q_OBJECT

public:
    explicit WeatherFetcher(QObject *parent = nullptr);
    QString getWeatherString(const QString &location); // 返回简洁天气信息

private slots:
    void onReplyFinished(QNetworkReply *reply); // 若后续用异步方式可用

private:
    QNetworkAccessManager *manager;
};

#endif // WEATHERFETCHER_H
