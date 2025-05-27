#include "use_webengine.h"
#ifdef USE_WEB

#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QWidget>
#include <QUrl>
#include <QWebEngineView>
#include <QEventLoop>

class QWebEngineView;
class QPushButton;

class BrowserWindow : public QWidget
{
    Q_OBJECT
public:
    explicit BrowserWindow(const QUrl &url, QWidget *parent = nullptr);

    // 静态函数创建浏览器窗口
    static void openBrowserAndSaveHtml(const QUrl &url, QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void saveAndClose();
    void injectOverlayButton();
    void handleNewWindowRequest(QWebEngineNewWindowRequest &request);

private:
    QWebEngineView *webView;
    QPushButton *overlayButton;
};

#endif // BROWSERWINDOW_H

#endif
