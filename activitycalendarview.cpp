#include "activitycalendarview.h"
#include <QDebug>
#include <QQueue>

ActivityCalendarView::ActivityCalendarView(QWidget *parent)
    : QWidget(parent)
{
    // 默认显示过去一年的数据
    m_endDate = QDate::currentDate();
    m_startDate = m_endDate.addYears(-2).addDays(1); // 从一年前开始
    setMinimumHeight(7 * (m_squareSize + m_spacing) + 20); // 确保有足够的高度显示一周7天
}


void ActivityCalendarView::setData(const QQueue<fanqie_data>& dataQueue)
{
    m_dailyActivityMinutes.clear();
    // 使用 QQueue 的迭代器

    for (QQueue<fanqie_data>::const_iterator it = dataQueue.constBegin(); it != dataQueue.constEnd(); ++it) {
        const fanqie_data& data = *it; // 解引用迭代器，获取 const 引用
        m_dailyActivityMinutes.insert(data.date, data.total_minute);
    }
    update(); // 数据更新后重绘
}

void ActivityCalendarView::setDateRange(const QDate& startDate, const QDate& endDate)
{
    m_startDate = startDate;
    m_endDate = endDate;
    update(); // 范围更新后重绘
}

QColor ActivityCalendarView::getActivityColor(int minutes) const
{
    if (minutes <= 0) return m_emptyColor;
    if (minutes < 30) return m_lowActivityColor;    // < 30分钟
    if (minutes < 90) return m_midActivityColor;    // < 90分钟
    return m_highActivityColor;                     // >= 90分钟 (1.5小时)
}


int ActivityCalendarView::getWeekLabelWidth() const
{
    // 预估最长的星期标签（如 "Thu"）的宽度
    QFontMetrics fm(font());
    return fm.horizontalAdvance("Wed") + 5; // 留一些额外空间
}

QMap<int, QString> ActivityCalendarView::getMonthStartColumns() const
{
    QMap<int, QString> monthStarts;
    QDate current = m_startDate;
    int currentColumn = 0;


    int firstDayOfWeek = m_startDate.dayOfWeek(); // Qt::Monday = 1, Qt::Sunday = 7
    int startOffset = (firstDayOfWeek == Qt::Sunday) ? 0 : firstDayOfWeek; // 0 for Sun, 1 for Mon...

    int startRow = (firstDayOfWeek == Qt::Sunday) ? 0 : firstDayOfWeek;


    QDate tempDate = m_startDate;
    while(tempDate.dayOfWeek() != Qt::Sunday && tempDate > m_startDate.addDays(-7)) { // 找到本周的周日
        tempDate = tempDate.addDays(-1);
    }
    int daysFromStartSunday = tempDate.daysTo(m_startDate); // 距离本周周日的天数
    currentColumn = daysFromStartSunday / 7; // 起始列


    QDate date = m_startDate;
    while (date <= m_endDate.addDays(7)) { // 稍微多一点，确保跨月
        if (date.day() == 1) { // 如果是每月的第一天
            // 计算当前日期所在的列
            QDate firstDayOfRange = m_startDate;
            while(firstDayOfRange.dayOfWeek() != Qt::Sunday && firstDayOfRange > m_startDate.addDays(-7)) {
                firstDayOfRange = firstDayOfRange.addDays(-1);
            }
            int totalDays = firstDayOfRange.daysTo(date);
            int column = totalDays / 7; // 从起始周日算起的列数

            // 确保只记录每个月的第一个出现的列
            if (!monthStarts.values().contains(date.toString("MMM"))) { // 避免重复月份标签
                monthStarts.insert(column, date.toString("MMM")); // 记录月份标签和它所在的列
            }
        }
        date = date.addDays(1);
    }
    return monthStarts;
}


void ActivityCalendarView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int weekLabelWidth = getWeekLabelWidth(); // 星期标签的宽度
    int monthLabelHeight = 15; // 月份标签的高度

    int startX = weekLabelWidth + m_spacing + 7; // 留出左侧星期标签和间距的空间
    int startY = monthLabelHeight + m_spacing; // 留出顶部月份标签和间距的空间

    // 绘制星期几标签 (Sun, Mon, ...)
    QStringList daysOfWeekLabels = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat","Sun"};
    painter.setPen(m_textColor);
    int weekLabelRightShift = 7; // 假设你想让星期标签右移5像素

    for (int i = 0; i < 7; ++i) { // 0=Sun, 1=Mon...
        // 原始的X坐标是0，现在加上 weekLabelRightShift
        painter.drawText(0 + weekLabelRightShift,
                         startY + i * (m_squareSize + m_spacing) + m_squareSize / 2 + 3,
                         daysOfWeekLabels.at(i));
    }

    // 绘制月份标签
    QMap<int, QString> monthStartColumns = getMonthStartColumns();
    QFontMetrics fm(font());
    painter.setPen(m_textColor);
    painter.setFont(QFont("Arial", 9));
    for (auto it = monthStartColumns.constBegin(); it != monthStartColumns.constEnd(); ++it) {
        int column = it.key();
        QString monthName = it.value();
        int x = startX + column * (m_squareSize + m_spacing);
        painter.drawText(x, monthLabelHeight - fm.descent() - 2, monthName); // 定位在方块上方
    }


    QDate currentDate = m_startDate;
    int currentColumn = 0; // 当前绘制的列（周）


    int firstBlockRowOffset = (m_startDate.dayOfWeek() == Qt::Sunday) ? 0 : m_startDate.dayOfWeek();


    while (currentDate <= m_endDate) {
        int dayOfWeek = currentDate.dayOfWeek(); // 1=Mon, ..., 7=Sun
        int row = (dayOfWeek == Qt::Sunday) ? 6 : (dayOfWeek - 1); // 将周日映射到第6行，周一到第0行，...周六到第5行


        QDate tempDate = m_startDate;
        while(tempDate.dayOfWeek() != Qt::Sunday && tempDate > m_startDate.addDays(-7)) { // 找到本周的周日
            tempDate = tempDate.addDays(-1);
        }
        int daysSinceStartSunday = tempDate.daysTo(currentDate); // 距离起始周日的天数
        currentColumn = daysSinceStartSunday / 7; // 当前日期所在的列



        int x = startX + currentColumn * (m_squareSize + m_spacing);
        int y = startY + row * (m_squareSize + m_spacing); // row 是 0-6

        // 绘制方格
        int minutes = m_dailyActivityMinutes.value(currentDate, 0);
        painter.setBrush(QBrush(getActivityColor(minutes)));
        painter.setPen(Qt::NoPen);
        painter.drawRect(x, y, m_squareSize, m_squareSize);

        currentDate = currentDate.addDays(1); // 移动到下一天
    }

    // 更新Widget的尺寸提示和最小尺寸，以适应绘制内容
    int totalWidth = startX + (currentColumn + 1) * (m_squareSize + m_spacing);
    setMinimumWidth(totalWidth);
    updateGeometry(); // 通知布局管理器尺寸提示已改变
}

QSize ActivityCalendarView::sizeHint() const
{
    // 重新计算 sizeHint
    int weekLabelWidth = getWeekLabelWidth();
    int monthLabelHeight = 15;

    // 计算需要多少列（周）来显示所有日期
    QDate tempStartDate = m_startDate;
    while(tempStartDate.dayOfWeek() != Qt::Sunday && tempStartDate > m_startDate.addDays(-7)) {
        tempStartDate = tempStartDate.addDays(-1);
    }
    int totalDaysToDraw = tempStartDate.daysTo(m_endDate);
    int numColumns = (totalDaysToDraw / 7) + 1; // 至少一列

    int suggestedWidth = weekLabelWidth + m_spacing + numColumns * (m_squareSize + m_spacing);
    int suggestedHeight = monthLabelHeight + m_spacing + 7 * (m_squareSize + m_spacing);

    return QSize(suggestedWidth, suggestedHeight);
}
