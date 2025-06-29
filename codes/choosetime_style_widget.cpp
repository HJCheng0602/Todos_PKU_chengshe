// choosetime_style_widget.cpp
#include "choosetime_style_widget.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QWheelEvent>
#include <QRegularExpression>

// CourseScrollWidget 实现
CourseScrollWidget::CourseScrollWidget(DisplayType type, int min, int max, QWidget *parent)
    : QWidget(parent), m_type(type), m_min(min), m_max(max),
    m_current(min), m_offset(0), m_dragging(false) {
    setFixedSize(120, 200);
}

void CourseScrollWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::white);

    const int centerY = 130 / 2;
    const int itemHeight = 40;

    // 绘制五个可见项
    for(int i = -2; i <= 2; ++i) {
        int value = m_current + i;
        // 数值循环处理
        while(value < m_min) value += (m_max - m_min + 1);
        while(value > m_max) value -= (m_max - m_min + 1);

        int yPos = centerY + (i * itemHeight) - m_offset;
        float alpha = 1.0 - (qAbs(yPos - centerY) / (itemHeight * 2.0f));

        painter.setPen(QColor(255, 255, 255, alpha * 200));
        painter.drawText(QRect(0, yPos - 20, width(), 40),
                         Qt::AlignCenter, getDisplayText(value));
    }

    // 绘制选中指示线
    painter.setPen(QColor(255, 255, 255, 150));
    painter.drawLine(0, centerY - 25, width(), centerY - 25);
    painter.drawLine(0, centerY + 25, width(), centerY + 25);
}

QString CourseScrollWidget::getDisplayText(int value) const {
    if(m_type == WeekDay) {
        const QString weekDays[] = {"周一","周二","周三","周四","周五","周六","周日"};
        return weekDays[value-1];
    }
    return QString("第%1节课").arg(value);
}

// void CourseScrollWidget::mousePressEvent(QMouseEvent *event) {
//     m_dragging = true;
//     m_lastPos = event->pos();
//     m_offset = 0;
// }

// void CourseScrollWidget::mouseMoveEvent(QMouseEvent *event) {
//     if(m_dragging) {
//         int delta = event->pos().y() - m_lastPos.y();
//         m_offset += delta;
//         m_lastPos = event->pos();
//         update();
//     }
// }

// void CourseScrollWidget::mouseReleaseEvent(QMouseEvent *) {
//     m_dragging = false;

//     const int threshold = 30;
//     if(m_offset > threshold) {  // 向下拖动
//         m_current++;  // 改为增加数值
//     } else if(m_offset < -threshold) { // 向上拖动
//         m_current--;  // 改为减少数值
//     }

//     // 边界检查
//     while(m_current < m_min) m_current += (m_max - m_min + 1);
//     while(m_current > m_max) m_current -= (m_max - m_min + 1);

//     animateToCenter();
//     emit valueChanged(m_current);
// }

void CourseScrollWidget::wheelEvent(QWheelEvent *event) {
    m_current += (event->angleDelta().y() > 0) ? -1 : 1;
    while(m_current < m_min) m_current += (m_max - m_min + 1);
    while(m_current > m_max) m_current -= (m_max - m_min + 1);
    emit valueChanged(m_current);
    animateToCenter();
    update();
}

void CourseScrollWidget::setOffset(int o) {
    m_offset = o;
    update();
}

void CourseScrollWidget::animateToCenter() {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "offset");
    animation->setDuration(400);
    animation->setEasingCurve(QEasingCurve::OutBack);
    animation->setStartValue(m_offset);
    animation->setEndValue(0);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

int CourseScrollWidget::value() const { return m_current; }
QString CourseScrollWidget::displayText() const { return getDisplayText(m_current); }

// 主界面实现
choosetime_style_widget::choosetime_style_widget(QWidget *parent) : QWidget(parent) {
    setStyleSheet("background-color: #1a1a1a;");
    setupSelectors();
}

void choosetime_style_widget::setupSelectors() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);  // 改为垂直布局

    // 时间选择器布局
    QHBoxLayout *selectorLayout = new QHBoxLayout();
    m_daySelector = new CourseScrollWidget(CourseScrollWidget::WeekDay, 1, 7);
    m_course1Selector = new CourseScrollWidget(CourseScrollWidget::Course, 1, 11);
    m_course2Selector = new CourseScrollWidget(CourseScrollWidget::Course, 1, 11);

    selectorLayout->addWidget(m_daySelector);
    selectorLayout->addWidget(m_course1Selector);
    selectorLayout->addWidget(m_course2Selector);
    selectorLayout->setSpacing(30);

    mainLayout->addLayout(selectorLayout);
    mainLayout->addStretch();  // 添加弹性空间

    setupButtons();  // 初始化按钮
}

QString choosetime_style_widget::selectedDay() const {
    return m_daySelector->displayText();
}

QString choosetime_style_widget::selectedCourse1() const {
    return m_course1Selector->displayText();
}

int extractCourseNumber(const QString& course){
    // 使用正则表达式提取数字部分
    QRegularExpression re("第(\\d+)节");
    QRegularExpressionMatch match = re.match(course);
    if (match.hasMatch()) {
        return match.captured(1).toInt();
    }
    return 0; // 无效值处理
}

QString choosetime_style_widget::selectedCourse2() const {
    return m_course2Selector->displayText();
}
void choosetime_style_widget::setupButtons() {
    // 创建按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(10, 0, 10, 10);  // 下边距10像素

    // 保存按钮
    m_saveButton = new QPushButton("保存", this);
    m_saveButton->setFixedSize(101, 31);
    m_saveButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #00CED1;"
        "    color: white;"
        "    border: none;"
        "    padding: 5px;"
        "    font-size: 13px;"
        "    border-radius: 5px;"
        "}"
        "QPushButton:hover { background-color: #00B2B2; }"
        "QPushButton:pressed { background-color: #008C8C; }"
        );

    // 取消按钮
    m_cancelButton = new QPushButton("取消", this);
    m_cancelButton->setFixedSize(101, 31);
    m_cancelButton->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    color: white;"
        "    padding: 5px;"
        "    font-size: 13px;"
        "    border-radius: 5px;"
        "}"
        "QPushButton:hover { background-color: rgba(0,0,0,220); }"
        "QPushButton:pressed { background-color: rgba(0,0,0,220); }"
        );

    // 添加到布局
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addSpacing(14);  // 按钮间距14像素（总间距125-10-101=14）
    buttonLayout->addWidget(m_cancelButton);

    // 将按钮布局添加到主布局
    qobject_cast<QVBoxLayout*>(layout())->addLayout(buttonLayout);

    // 连接信号（示例）
    connect(m_saveButton, &QPushButton::clicked, this, [=](){
        qDebug() << "保存时间选择："
                 << selectedDay()
                 << selectedCourse1()[1]
                 << selectedCourse2();

        QVector<int>bubu;
        QString weekday[] = {"周一", "周二", "周三", "周四", "周五", "周六", "周日"};
        for(int i = 0; i < 7; i++)
        {
            if(weekday[i] == selectedDay())
            {
                bubu.push_back(i + 1);
                break;
            }
        }
        int course1 = extractCourseNumber(selectedCourse1());
        int course2 = extractCourseNumber(selectedCourse2());

        bubu.push_back(course1);

        if(course1 <= course2) {
            bubu.push_back(course2);
        } else {
            bubu.push_back(course1);
        }
        emit datasaved(bubu);
        this->close();
    });

    connect(m_cancelButton, &QPushButton::clicked, this, [=](){
        this->close();
    });
}


void choosetime_style_widget::clear_redraw() {
    // 重设三个选择器的值为最小值
    m_daySelector->setValue(1);
    m_course1Selector->setValue(1);
    m_course2Selector->setValue(1);

    // 更新显示（强制重绘）
    m_daySelector->update();
    m_course1Selector->update();
    m_course2Selector->update();
}


void CourseScrollWidget::setValue(int v) {
    // 边界处理
    while(v < m_min) v += (m_max - m_min + 1);
    while(v > m_max) v -= (m_max - m_min + 1);
    m_current = v;
    emit valueChanged(m_current);
    animateToCenter();
    update();
}

