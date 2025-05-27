#ifndef DATECALENDAR_H
#define DATECALENDAR_H

#include <QWidget>
#include<QDate>

namespace Ui {
class datecalendar;
}

class datecalendar : public QWidget
{
    Q_OBJECT

public:
    explicit datecalendar(QWidget *parent = nullptr);
    ~datecalendar();
    bool isvi;
    void draw_calendar(QDate date);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_52_clicked();

    void on_pushButton_51_clicked();

    void on_saveButton_clicked();


    void on_timeEdit_userTimeChanged(const QTime &time);

private:
    Ui::datecalendar *ui;
    QDate nowdate;
    QDate selected_date;
    QTime selected_time = QTime(0,0);

    void connectButtons();
    void initCalendarButtons();
signals:
    void data_saved(QDate &selected_date);
    void datas_saved(QPair<QDate, QTime> & selected);
};

#endif // DATECALENDAR_H
