#ifndef SMALLWIDGET_H
#define SMALLWIDGET_H

#include <QWidget>
#include<QVector>
#include"custombutton.h"
#include"smallcustombutton.h"
#include"datecalendar.h"
#include"subrepeat.h"
#include<QMenu>


namespace Ui {
class smallwidget;
}

class Widget;

class smallwidget : public QWidget
{
    Q_OBJECT

public:
    explicit smallwidget(QWidget *parent = nullptr);
    ~smallwidget();
    QVector<CustomButton *> * created_large_buttons = new QVector<CustomButton *>;
    QVector<SmallCustomButton*> * created_small_buttons = new QVector<SmallCustomButton *>;
    datecalendar w;
    datecalendar v;
    subrepeat c;
    QDate todo_set_date = QDate::currentDate();
    Widget * po;

    QString line; //放到搜索框右边的信息
    QString repeat_info = "无循环";
    bool is_todo_date_changed = false;
    QTime todo_set_time = QTime::currentTime();
    bool is_todo_time_changed = false;
    int now_edit_bbutton = -1;

    int unitnum = 0;
    int unittype = -1;
    void draw_todo_buttons_from_vector();

    void change_background(int id);


private:
    Ui::smallwidget *ui;
public slots:
    void open_edit_menu(const QString &id);
    void mark_finished(const QString &id);
    void mark_deleted(const QString &id);
private slots:

    void updateButtonVisibility(const QString &text);
    void onTodayActionTriggered();
    void onTomorrowActionTriggered();
    void onChooseDateActionTriggered();
    void onTodaytimeActionTriggered();
    void onTomorrowtimeActionTriggered();
    void onChooseDatetimeActionTriggered();

    void onEverydayActionTriggered();
    void onEveryweekActionTriggered();
    void onSelfdefineActionTriggered();
    void on_addtodo_returnPressed();
    void on_close_edit_button_clicked();
    void on_edit_todo_title_textChanged(const QString &arg1);
    void on_set_todo_finished_clicked();
    void on_add_todo_info_textChanged();
    void on_reset_todo_time_clicked();
    void on_reset_todo_date_clicked();
    void on_reset_todo_repeat_clicked();
    void on_delete_todo_clicked();
    void on_chagesize_clicked();

signals:
    void changesizeButtonClicked();
};

#endif // SMALLWIDGET_H
