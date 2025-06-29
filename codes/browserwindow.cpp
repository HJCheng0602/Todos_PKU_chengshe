#include "use_webengine.h"
#ifdef USE_WEB
#include "widget.h"
#include "header.h"
#include <QWebEnginePage>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QTimer>
#include <QWebEngineSettings>
#include <QWebEngineNewWindowRequest>

BrowserWindow::BrowserWindow(const QUrl &url, QWidget *parent)
    : QWidget(parent)
{
    // 设置窗口属性
    setWindowTitle("Login->My Timetable->save");
    resize(800, 600);

    // 创建浏览器视图
    webView = new QWebEngineView(this);
    webView->setUrl(url);

    // 连接新窗口请求信号
    connect(webView->page(), &QWebEnginePage::newWindowRequested,
            this, &BrowserWindow::handleNewWindowRequest);

    // 在BrowserWindow构造函数中添加：
    webView->page()->setDevToolsPage(webView->page()); // 允许开发者工具
    webView->page()->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    webView->page()->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    // connect(webView->page(), &QWebEnginePage::navigationRequested,
    //         this, [](const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame) {
    //             qDebug() << "Navigation requested:" << url << "Type:" << type;
    //         });
    // 创建悬浮按钮
    overlayButton = new QPushButton("save", this);
    overlayButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   border: none;"
        "   padding: 10px 20px;"
        "   font-size: 16px;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
        );
    overlayButton->setFixedSize(120, 40);
    overlayButton->raise(); // 确保按钮在最上层

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(webView);

    // 连接按钮信号
    connect(overlayButton, &QPushButton::clicked, this, &BrowserWindow::saveAndClose);

    // 页面加载完成后注入悬浮按钮样式
    connect(webView, &QWebEngineView::loadFinished, this, &BrowserWindow::injectOverlayButton);
}
// 修改后的处理函数
void BrowserWindow::handleNewWindowRequest(QWebEngineNewWindowRequest &request)
{
    webView->page()->setUrl(request.requestedUrl());
}
void BrowserWindow::openBrowserAndSaveHtml(const QUrl &url, QWidget *parent)
{
    BrowserWindow *browser = new BrowserWindow(url, parent);
    QEventLoop loop;
    QObject::connect(browser, &BrowserWindow::destroyed, &loop, &QEventLoop::quit);
    browser->setAttribute(Qt::WA_DeleteOnClose);
    browser->show();
    loop.exec();
    return;
}

void BrowserWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // 保持按钮在右下角
    overlayButton->move(width() - overlayButton->width() - 20,
                        height() - overlayButton->height() - 20);
}


void BrowserWindow::saveAndClose()
{
    // 获取网页HTML内容
    webView->page()->toHtml([this](const QString &html) {
        // 保存到文件
        QFile file(Widget::getDataPath("saved_page.html"));
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << html;
            file.close();
            QMessageBox::information(this, "成功", "网页已保存");
        } else {
            QMessageBox::warning(this, "错误", "无法保存文件");
        }

        // 延迟关闭以避免竞争条件
        QTimer::singleShot(100, this, &QWidget::close);
    });
}

void BrowserWindow::injectOverlayButton()
{
    // 注入CSS确保我们的按钮不会被页面内容覆盖
    webView->page()->runJavaScript(
        "var style = document.createElement('style');"
        "style.type = 'text/css';"
        "style.innerHTML = 'body { margin-bottom: 50px !important; }';"
        "document.head.appendChild(style);"
        );
}


#endif
