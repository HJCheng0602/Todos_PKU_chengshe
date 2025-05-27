#ifndef LINEARPROGRESSBAR_H
#define LINEARPROGRESSBAR_H

#include <QWidget>
#include <QColor>

class LinearProgressBar : public QWidget
{
    Q_OBJECT

public:
    explicit LinearProgressBar(QWidget *parent = nullptr);

    void setProgress(float progress); // 0.0 ~ 1.0

    int colorwho = 0; // 0:番茄色 1:休息色
    QColor redcolor = QColor(255, 102, 102); // 番茄色
    QColor greencolor = QColor(102, 204, 0); // 休息色

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    float m_progress; // 当前进度
};

#endif // LINEARPROGRESSBAR_H
