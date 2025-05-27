#ifndef DEEPSEEKCLIENT_H
#define DEEPSEEKCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class DeepSeekClient : public QObject
{
    Q_OBJECT
public:
    explicit DeepSeekClient(QObject *parent = nullptr);

    void setApiKey(const QString &key);
    void sendPrompt(const QString &prompt);
    void sendPrompt(const QVector<QPair<QString, QString>> &messageHistory);

signals:
    void responseReady(const QString &response);
    void errorOccurred(const QString &error);

private:
    QNetworkAccessManager *manager;
    QString apiKey;
};

#endif // DEEPSEEKCLIENT_H
