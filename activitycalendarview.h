#ifndef ACTIVITYCALENDARVIEW_H
#define ACTIVITYCALENDARVIEW_H

#include <QWidget>
#include <QDate>
#include <QMap> // 用于存储每天的活动数据，方便查找
#include <QPainter> // 绘图

// 你的数据结构
struct fanqie_data
{
    QDate date;
    QVector<QPair<QTime, QTime>> fanqie_slots_paired;
    int total_minute;
};

class ActivityCalendarView : public QWidget
{
    Q_OBJECT
public:
    explicit ActivityCalendarView(QWidget *parent = nullptr);

    // 设置数据，需要将QQueue转换为QMap方便查找
    void setData(const QQueue<fanqie_data>& dataQueue);
    // 设置显示的时间范围 (例如，从哪个月到哪个月)
    void setDateRange(const QDate& startDate, const QDate& endDate);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override; // 建议的尺寸

private:
    QMap<QDate, int> m_dailyActivityMinutes; // 存储每天的总分钟数
    QDate m_startDate; // 显示范围的开始日期
    QDate m_endDate;   // 显示范围的结束日期

    int m_squareSize = 12; // 每个方格的大小
    int m_spacing = 3;     // 方格之间的间距
    QColor m_emptyColor = QColor(50, 50, 50); // 空白方格颜色
    QColor m_lowActivityColor = QColor(30, 80, 150); // 低活跃度颜色 (浅蓝)
    QColor m_midActivityColor = QColor(40, 120, 200); // 中活跃度颜色 (中蓝)
    QColor m_highActivityColor = QColor(50, 160, 250); // 高活跃度颜色 (亮蓝)
    QColor m_textColor = Qt::lightGray;

    int getWeekLabelWidth() const;
    // 新增：获取月份标签的起始列
    QMap<int, QString> getMonthStartColumns() const;

    // 根据分钟数获取颜色
    QColor getActivityColor(int minutes) const;
};

#endif // ACTIVITYCALENDARVIEW_H
