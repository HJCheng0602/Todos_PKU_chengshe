#include "deepseekclient.h"
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkProxy>
#include <QDebug>

DeepSeekClient::DeepSeekClient(QObject *parent)
    : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    manager->setProxy(QNetworkProxy::NoProxy);  // 如无代理需求，可保持 NoProxy
}

void DeepSeekClient::setApiKey(const QString &key)
{
    this->apiKey = key;
}

void DeepSeekClient::sendPrompt(const QString &prompt)
{
    QUrl url("https://api.deepseek.com/v1/chat/completions");
    QNetworkRequest request(url);
    qDebug() << "Sending request to DeepSeek API with prompt:" << prompt;
    qDebug() << apiKey;
    qDebug() << apiKey.isEmpty();
    if (apiKey.isEmpty()) {
        emit errorOccurred("API Key 未设置");

        qDebug() << "here";
        return;
    }

    request.setRawHeader("Authorization", "Bearer " + apiKey.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonArray messages;
    QJsonObject userMessage;
    userMessage["role"] = "user";
    userMessage["content"] = prompt;
    messages.append(userMessage);

    QJsonObject payload;
    payload["model"] = "deepseek-chat";  // DeepSeek v3 模型标识
    payload["messages"] = messages;
    payload["temperature"] = 0.7;

    QJsonDocument doc(payload);
    QByteArray data = doc.toJson();

    QNetworkReply *reply = manager->post(request, data);

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject obj = jsonResponse.object();
            QString result = obj["choices"].toArray()[0].toObject()["message"].toObject()["content"].toString();
            emit responseReady(result);
        } else {
            emit errorOccurred("请求失败: " + reply->errorString());
        }
        reply->deleteLater();
    });

    qDebug() << "Request sent to DeepSeek API.";
}


void DeepSeekClient::sendPrompt(const QVector<QPair<QString, QString>> &messageHistory)
{
    QUrl url("https://api.deepseek.com/v1/chat/completions");
    QNetworkRequest request(url);
    qDebug() << apiKey;
    qDebug() << apiKey.isEmpty();
    if (apiKey.isEmpty()) {
        emit errorOccurred("API Key 未设置");

        qDebug() << "here";
        return;
    }

    request.setRawHeader("Authorization", "Bearer " + apiKey.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonArray messages;
    for (const auto &pair : messageHistory) {
        QJsonObject msg;
        msg["role"] = pair.first;
        msg["content"] = pair.second;
        messages.append(msg);
    }

    QJsonObject payload;
    payload["model"] = "deepseek-chat";
    payload["messages"] = messages;
    payload["temperature"] = 0.7;

    QJsonDocument doc(payload);
    QByteArray data = doc.toJson();

    QNetworkReply *reply = manager->post(request, data);

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject obj = jsonResponse.object();
            QString result = obj["choices"].toArray()[0].toObject()["message"].toObject()["content"].toString();
            emit responseReady(result);
        } else {
            emit errorOccurred("请求失败: " + reply->errorString());
        }
        reply->deleteLater();
    });

    qDebug() << "Request sent with multi-message format.";
}
