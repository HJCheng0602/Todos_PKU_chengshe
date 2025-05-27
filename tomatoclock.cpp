#include "tomatoclock.h"
#include "ui_tomatoclock.h"
#include <QDebug>

TomatoClock::TomatoClock(QWidget *parent, QTimer *externalTimer)
    : QWidget(parent), ui(new Ui::TomatoClock)
{
    ui->setupUi(this);
    boundaryWidth = 4;
    Opacity = 0.8;
    current_color = "background:#CD9B9B";
    setWindowFlags(Qt::FramelessWindowHint);
    setMinimumSize(45, 45);
    setStyleSheet(current_color);
    setWindowOpacity(Opacity);

    ui->Timer->setDigitCount(5);
    progress = 0;

    currentMode = Work;
    currentRunState = Running;

    workDurationSecs = 25 * 60;
    restDurationSecs = 5 * 60;

    if (externalTimer) {
        timer = externalTimer;
        useExternalTimer = true;
    } else {
        timer = new QTimer(this);
        useExternalTimer = false;
    }

    connect(timer, &QTimer::timeout, this, &TomatoClock::updateTime);
    timer->setInterval(1000);
    timer->start();

    resetTime();
}

TomatoClock::~TomatoClock() {
    if (!useExternalTimer && timer) {
        delete timer;
    }
    delete ui;
}

void TomatoClock::resetTime() {
    progress = 0;
    time.setHMS(0, 0, 0);
    ui->Timer->display(time.toString("mm:ss"));
    update();
}

void TomatoClock::start() {
    if (currentRunState != Running) {
        currentRunState = Running;
        if (!timer->isActive()) timer->start();
        Opacity = 0.8;
        updateStyle();
    }
}

// 可配合 pause() 函数使用
void TomatoClock::pause() {
    if (currentRunState != Paused) {
        currentRunState = Paused;
        if (timer->isActive()) timer->stop();
        Opacity = 0.4;
        updateStyle();
    }
}

void TomatoClock::updateExternalTime(int seconds) {
    progress = seconds;
    time.setHMS(0, 0, 0);
    time = time.addSecs(seconds);
    ui->Timer->display(time.toString("mm:ss"));
    update();
}

void TomatoClock::setState(TomatoState mode, RunState running) {
    currentMode = mode;
    currentRunState = running;

    if (currentRunState == Paused) {
        if (timer->isActive()) timer->stop();
        Opacity = 0.4;
    } else {
        if (!timer->isActive()) timer->start();
        Opacity = 0.8;
    }

    updateStyle();
}

void TomatoClock::updateStyle() {
    if (currentRunState == Paused) {
        setStyleSheet("background:#B5B5B5");
    } else {
        current_color = (currentMode == Work) ? "background:#CD9B9B" : "background:#9BCD9B";
        setStyleSheet(current_color);
    }
    setWindowOpacity(Opacity);
}

void TomatoClock::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton)
        clickPos = e->pos();

    if (currentRunState == Running)
        setState(currentMode, Paused);
    else
        setState(currentMode, Running);

    emit tomatoClockstop();
}

void TomatoClock::mouseMoveEvent(QMouseEvent *e) {
    if (e->buttons() & Qt::LeftButton)
        move(e->pos() + pos() - clickPos);
}

void TomatoClock::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height()) - 60;
    QRectF rect(30, 30, side, side);
    float angle;

    int duration = (currentMode == Work) ? workDurationSecs : restDurationSecs;
    angle = progress * 360.0 / duration;

    painter.setPen(QPen(QColor(200, 200, 200), 25));
    painter.drawArc(rect, 0, 360 * 16);

    QColor progressColor = (current_color == "background:#CD9B9B") ?
                               QColor(255, 102, 102) :
                               QColor(102, 204, 0);
    painter.setPen(QPen(progressColor, 25, Qt::SolidLine, Qt::RoundCap));
    painter.drawArc(rect, 90 * 16, -angle * 16);
}

void TomatoClock::updateTime() {
    if (currentRunState == Paused) return;

    time = time.addSecs(1);
    progress++;
    ui->Timer->display(time.toString("mm:ss"));

    if (progress % 10 == 0) update();

    int currentDuration = (currentMode == Work) ? workDurationSecs : restDurationSecs;

    if (progress >= currentDuration) {
        emit roundFinished(currentMode);

        TomatoState nextMode = (currentMode == Work) ? Rest : Work;
        setState(nextMode, Running);
        resetTime();
    }
}

void TomatoClock::setWorkDuration(int minutes) {
    workDurationSecs = minutes * 60;
    if (currentMode == Work) resetTime();
}

void TomatoClock::setRestDuration(int minutes) {
    restDurationSecs = minutes * 60;
    if (currentMode == Rest) resetTime();
}

void TomatoClock::on_closeButton_clicked() {
    emit windowClosed();
    close();
}
