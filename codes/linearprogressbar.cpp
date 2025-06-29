#include "LinearProgressBar.h"
#include <QPainter>
#include <QPainterPath>

LinearProgressBar::LinearProgressBar(QWidget *parent)
    : QWidget(parent), m_progress(0.0f)
{
    setMinimumHeight(20);
    setMaximumHeight(20);
}

void LinearProgressBar::setProgress(float progress)
{
    m_progress = qBound(0.0f, progress, 1.0f);
    update();
}

void LinearProgressBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF fullRect = rect().adjusted(1, 1, -1, -1);
    qreal radius = fullRect.height() / 2.0;

    // 背景条
    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(fullRect, radius, radius);
    painter.fillPath(backgroundPath, QColor("#e0e0e0"));

    // 进度条
    QRectF progressRect = fullRect;
    progressRect.setWidth(fullRect.width() * m_progress);

    QPainterPath progressPath;
    if (m_progress < 1.0)
        progressPath.addRoundedRect(progressRect, radius, radius);
    else
        progressPath.addRoundedRect(fullRect, radius, radius); // 满进度时防止右边不圆
    if (colorwho == 0)
        painter.fillPath(progressPath, redcolor); // 番茄色
    else if (colorwho == 1)
        painter.fillPath(progressPath, greencolor); // 休息色
}
