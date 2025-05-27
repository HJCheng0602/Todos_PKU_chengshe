#ifndef EDITCLASSWIDGET_H
#define EDITCLASSWIDGET_H

#include <QWidget>
#include <QVector>
#include "chooseweek_style_widget.h"
#include <QColor>
#include <QPalette>
#include <QString>
#include <QTime>
#include "choosetime_style_widget.h"


namespace Ui {
class editclasswidget;
}


struct a_class
{
    QVector<bool>weekinfo;
    QString classname;
    QString classinfo;
    QString classid;
    QColor thecolor;
    int which_weekday = 1;
    bool use_precise_time = false;
    QTime begintime;
    QTime endtime;
    int beginclass = 1;
    int endclass = 1;
};

class editclasswidget : public QWidget
{
    Q_OBJECT

public:
    explicit editclasswidget(QWidget *parent = nullptr);
    QPalette a;
    chooseweek_style_widget chooseweek;
    QVector<bool>weekinfo;
    QString classid;
    QString classname;
    QString classinfo; // 备注
    QColor thecolor;
    int which_weekday = 1; //default : Mon
    bool use_precise_time = false;
    QTime begintime;
    QTime endtime;
    int begin_class = 1;
    int end_class = 1;
    choosetime_style_widget q;
    void clear();
    void redraw();
    bool is_hide_delete = true;
    void paintEvent(QPaintEvent *event);

    ~editclasswidget();

private slots:
    void on_change_week_style_clicked();

    void on_edit_class_name_line_textChanged(const QString &arg1);

    void on_extro_info_editline_textChanged(const QString &arg1);

    void on_change_time_style_clicked();

    void on_changecolorbutton_clicked();

    void on_pushButton_51_clicked();

    void on_saveButton_clicked();

    void on_delete_class_clicked();

signals:
    void data_saved(a_class* oi);
    void data_canceled();

private:
    Ui::editclasswidget *ui;
};

#endif // EDITCLASSWIDGET_H
