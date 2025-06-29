#include "datecalendar.h"
#include "ui_datecalendar.h"
#include<QDate>
#include <QDateTime>
#include<QDebug>
#include<QPair>
#include<QSettings>



datecalendar::datecalendar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::datecalendar), isvi(false)
{
    setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    initCalendarButtons();
    nowdate = QDate::currentDate(); // 假设这是已有的QDate对象
    selected_date = nowdate;
    QDate currentDate = QDate::currentDate();
    QDate firstDay(currentDate.year(), currentDate.month(), 1);
    QDate startDate = firstDay.addDays(-(firstDay.dayOfWeek() - 1)); // 确保从周一开始
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",QSettings::NativeFormat);
    if(settings.value("AppsUseLightTheme")==0){
        ui->selectdate->setStyleSheet("background-color: transparent;"
            "color: white");
    } else {
        ui->selectdate->setStyleSheet("background-color: transparent;"
                                 "       color: black");
        ui->pushButton_51->setStyleSheet("QPushButton {"
    "background-color: transparent;"
    "padding: 5px 5px; /* 内边距 */"
    "font-size: 13px; /* 字体大小 */"
    "border-radius: 5px; /* 圆角 */"
"}"

"QPushButton:hover {"
    "background-color: rgba(0,0, 0, 80);"
"}"

"QPushButton:pressed {"
    "background-color:  rgba(0, 0, 0,100);"
"}"
                                         );
    }


    for (int i = 1; i <= 49; ++i) {  // 关键修改：从1遍历到49
        QPushButton* btn = ui->groupBox->findChild<QPushButton*>(
            "pushButton_" + QString::number(i));

        if (btn) {
            QDate date = startDate.addDays(i-1);  // 关键修改：i-1对应日期偏移
            btn->setText(date.month() == firstDay.month()
                             ? QString::number(date.day())
                             : "");
            if(date.month() != firstDay.month())
            {
                btn->setVisible(false);
            }
        }

    }
    ui->selectdate->setText(nowdate.toString("yyyy-MM"));
    connectButtons();
    //connect(ui->pushButton, &QPushButton::clicked, this, &datecalendar::on_pushButton_clicked);
}



datecalendar::~datecalendar()
{
    delete ui;
}

void datecalendar::on_pushButton_clicked()
{
    selected_date =  selected_date.addMonths(-1);

    draw_calendar(selected_date);

}

void datecalendar::draw_calendar(QDate date)
{
    ui->time->setVisible(isvi);
    ui->timeEdit->setVisible(isvi);
    QDate currentDate = date;
    QDate firstDay(currentDate.year(), currentDate.month(), 1);
    QDate startDate = firstDay.addDays(-(firstDay.dayOfWeek() - 1)); // 确保从周一开始
    initCalendarButtons();

    for (int i = 1; i <= 49; ++i) {  // 关键修改：从1遍历到49
        QPushButton* btn = ui->groupBox->findChild<QPushButton*>(
            "pushButton_" + QString::number(i));

        if (btn) {
            QDate date = startDate.addDays(i-1);  // 关键修改：i-1对应日期偏移
            btn->setText(date.month() == firstDay.month()
                             ? QString::number(date.day())
                             : "");
            if(date.month() != firstDay.month())
            {
                btn->setVisible(false);
            }
        }
    }
    ui->selectdate->setText(currentDate.toString("yyyy-MM"));
}


void datecalendar::on_pushButton_52_clicked()
{
    qDebug() <<"下载尝试\n";
    selected_date =  selected_date.addMonths(1);
    draw_calendar(selected_date);

}



void datecalendar::initCalendarButtons()
{
    for (int i = 1; i <= 49; ++i) {
        QPushButton* btn = ui->groupBox->findChild<QPushButton*>("pushButton_" + QString::number(i));

        if (btn) {

            // 设置默认样式
            btn->setStyleSheet("background:transparent;");
            btn->setVisible(true);
        }
    }
}

void datecalendar::connectButtons()
{



    for (int i = 1; i <= 49; ++i) {
        QPushButton* btn = ui->groupBox->findChild<QPushButton*>("pushButton_" + QString::number(i));

        if (btn) {
            // 存储按钮的索引作为属性
            btn->setProperty("index", i);

            // 连接每个按钮的 clicked 信号到槽函数
            connect(btn, &QPushButton::clicked, [btn, this, i]() {
                btn->setChecked(!btn->isChecked());
                if (btn->isChecked()) {
                    // 设置选中样式
                    btn->setStyleSheet("QPushButton::checked { background-color: #1BCBDE; color: white; }");
                    QDate firstDay(selected_date.year(), selected_date.month(), 1);
                    int dayweek = firstDay.dayOfWeek();
                    firstDay = firstDay.addDays(- dayweek + 1);
                    // 计算选中的日期

                    selected_date.setDate(selected_date.year(), selected_date.month(), 1 - dayweek + i);
                    // 输出选中的日期
                    qDebug() << "选中的日期：" << selected_date.toString("yyyy-MM-dd");
                } else {
                    // 恢复未选中样式
                    btn->setStyleSheet("background:transparent;");
                }
            });
        }
    }
}


void datecalendar::on_pushButton_51_clicked()
{
    close();
}


void datecalendar::on_saveButton_clicked()
{
    if(!isvi)
    {
        emit data_saved(selected_date);
        close();

    }
    else {
        QPair<QDate, QTime>pair(selected_date, selected_time);
        emit datas_saved(pair);
        close();

    }

}


void datecalendar::on_timeEdit_userTimeChanged(const QTime &time)
{
    selected_time = time;
    qDebug()<< "当前时间为:"<<selected_time.toString("hh:mm:ss");
}

