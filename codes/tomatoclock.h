#ifndef TOMATOCLOCK_H
#define TOMATOCLOCK_H

#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QTime>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class TomatoClock; }
QT_END_NAMESPACE

class TomatoClock : public QWidget
{
    Q_OBJECT

public:
    enum TomatoState { Work, Rest };
    enum RunState { Running, Paused };

    explicit TomatoClock(QWidget *parent = nullptr, QTimer *externalTimer = nullptr);
    ~TomatoClock();

    void setState(TomatoState mode, RunState running);
    void resetTime();
    void pause(); // 新增：暂停函数
    void start();
    void updateExternalTime(int seconds); // 新增：外部更新时间

    void setWorkDuration(int minutes);
    void setRestDuration(int minutes);

signals:
    void windowClosed();
    void roundFinished(TomatoClock::TomatoState finishedMode);
    void tomatoClockstop();

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateTime();
    void on_closeButton_clicked();

private:
    void updateStyle();
    void updateProgressArc();

    Ui::TomatoClock *ui;
    QTimer *timer;
    bool useExternalTimer;

    QPoint clickPos;
    int boundaryWidth;
    float Opacity;

    QTime time;
    int progress;

    TomatoState currentMode;
    RunState currentRunState;

    int workDurationSecs;
    int restDurationSecs;

    QString current_color;
};

#endif // TOMATOCLOCK_H
