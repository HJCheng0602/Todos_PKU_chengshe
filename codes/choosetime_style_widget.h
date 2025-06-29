// choosetime_style_widget.h
#ifndef CHOOSETIME_STYLE_WIDGET_H
#define CHOOSETIME_STYLE_WIDGET_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QVector>

class CourseScrollWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int offset READ offset WRITE setOffset)
public:
    enum DisplayType { WeekDay, Course };

    explicit CourseScrollWidget(DisplayType type, int min, int max, QWidget *parent = nullptr);
    int value() const;
    QString displayText() const;

    void setValue(int v);

signals:
    void valueChanged(int value);

protected:
    void paintEvent(QPaintEvent *event) override;
    // void mousePressEvent(QMouseEvent *event) override;
    // void mouseMoveEvent(QMouseEvent *event) override;
    // void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    DisplayType m_type;
    int m_min;
    int m_max;
    int m_current;
    int m_offset;
    QPoint m_lastPos;
    bool m_dragging;

    int offset() const { return m_offset; }
    void setOffset(int o);
    void updateValue();
    void animateToCenter();
    QString getDisplayText(int value) const;

};

class choosetime_style_widget : public QWidget {
    Q_OBJECT

public:
    explicit choosetime_style_widget(QWidget *parent = nullptr);

    QString selectedDay() const;     // 返回"周一"~"周日"
    QString selectedCourse1() const; // 返回"第一节课"~"第十一节课"
    QString selectedCourse2() const;
    void clear_redraw();  // 清空并重绘

signals:
    void selectionChanged();
    void datasaved(QVector<int>);

private:
    CourseScrollWidget *m_daySelector;
    CourseScrollWidget *m_course1Selector;
    CourseScrollWidget *m_course2Selector;
    void setupSelectors();
    QPushButton *m_saveButton;
    QPushButton *m_cancelButton;
    void setupButtons();  // 新增按钮初始化方法

    int day;
    int beginclass;
    int endclass;
};

#endif // CHOOSETIME_STYLE_WIDGET_H
