#include "editclasswidget.h"
#include "ui_editclasswidget.h"
#include "chooseweek_style_widget.h"
#include <QDebug>
#include "choosetime_style_widget.h"
#include <vector>
#include <random>
#include <cmath>
#include <string>
#include <QMessageBox>
#include <QTime>
#include <QPainter>

QString weekday[] = {"周一", "周二", "周三", "周四", "周五", "周六", "周日"};


std::vector<int> generateRichColor() {
    std::vector<int> rgb;

    std::random_device rd;
    std::mt19937 generator(rd());


    // 随机选择生成哪个类型的颜色
    std::uniform_int_distribution<int> typeDist(0, 4);  // 0-4 共5种类型
    int type = typeDist(generator);

    if (type == 0) {
        // 普通随机颜色
        std::uniform_int_distribution<int> dist(0, 255);
        rgb.push_back(dist(generator));
        rgb.push_back(dist(generator));
        rgb.push_back(dist(generator));
    }
    else if (type == 1) {
        // 暗色
        std::uniform_int_distribution<int> dist(0, 127);
        rgb.push_back(dist(generator));
        rgb.push_back(dist(generator));
        rgb.push_back(dist(generator));
    }
    else if (type == 2) {
        // 亮色
        std::uniform_int_distribution<int> dist(128, 255);
        rgb.push_back(dist(generator));
        rgb.push_back(dist(generator));
        rgb.push_back(dist(generator));
    }
    else if (type == 3) {
        // 互补色生成（返回互补色对）
        std::uniform_int_distribution<int> dist(0, 255);
        int r = dist(generator);
        int g = dist(generator);
        int b = dist(generator);

        rgb.push_back(r);
        rgb.push_back(g);
        rgb.push_back(b);

        rgb.push_back(255 - r);
        rgb.push_back(255 - g);
        rgb.push_back(255 - b);
    }
    else {  // type == 4
        // 相近色
        std::uniform_int_distribution<int> dist(0, 255);
        std::uniform_int_distribution<int> baseDist(0, 255);

        // 选择一个基准色
        int baseR = baseDist(generator);
        int baseG = baseDist(generator);
        int baseB = baseDist(generator);

        // 基于基准色生成相近色
        std::uniform_int_distribution<int> rangeDist(10, 50);
        int range = rangeDist(generator);

        int rLow = std::max(0, baseR - range);
        int rHigh = std::min(255, baseR + range);
        int gLow = std::max(0, baseG - range);
        int gHigh = std::min(255, baseG + range);
        int bLow = std::max(0, baseB - range);
        int bHigh = std::min(255, baseB + range);

        std::uniform_int_distribution<int> rDist(rLow, rHigh);
        std::uniform_int_distribution<int> gDist(gLow, gHigh);
        std::uniform_int_distribution<int> bDist(bLow, bHigh);

        rgb.push_back(baseR);
        rgb.push_back(baseG);
        rgb.push_back(baseB);

        rgb.push_back(rDist(generator));
        rgb.push_back(gDist(generator));
        rgb.push_back(bDist(generator));
    }

    return rgb;
}

editclasswidget::editclasswidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::editclasswidget)
{
    ui->setupUi(this);
    setFixedSize(size());
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    QTime current = QTime::currentTime();
    classid = current.toString("hhmmss") + QDate::currentDate().toString("MMdd");

    //qDebug() << "id :" << classid;


    if(is_hide_delete)
    {
        ui->delete_class->setVisible(false);
    }
    else {
        ui->delete_class->setVisible(true);
    }

    for(int i = 0; i < 20; i++)
    {
        this->weekinfo.push_back(true);
    }
    ui->change_week_style->setText(QString("每周"));
    ui->change_time_style->setText(QString("周一, 第1-1节"));
    std::vector<int>bubu = generateRichColor();
    ui->pushButton_5->setStyleSheet(
        "background-color: transparent;"
        "color:rgb(" + QString::number(bubu[0]) + "," + QString::number(bubu[1]) + "," + QString::number(bubu[2]) + ");"
        );
    thecolor = QColor(bubu[0], bubu[1] , bubu[2]);


    connect(&chooseweek, &chooseweek_style_widget::data_saved,[this](QVector<bool>  weekinfo)
            {
        this->weekinfo = weekinfo;
        bool isodd = true;
        bool iseven = true;
        for(int i = 0; i < 20; i++)
        {
            if(this->weekinfo[i] == true && i % 2 == 0)
            {
                iseven = false;
            }
            if(this->weekinfo[i] == true && i % 2 == 1)
            {
                isodd = false;
            }

        }

        if(iseven && !isodd)
        {
            ui->change_week_style->setText(QString("双周"));
        }
        else if(isodd && !iseven){
            ui->change_week_style->setText(QString("单周"));
        }
        else if(!isodd && !iseven)
        {
            ui->change_week_style->setText(QString("每周/自定义"));
        }
    });

    connect(&q, &choosetime_style_widget::datasaved, [this](QVector<int> timeinfo)
    {
        which_weekday = timeinfo[0];
        begin_class = timeinfo[1];
        end_class = timeinfo[2];
        ui->change_time_style->setText(weekday[which_weekday - 1] + QString(" 第") + QString((char)(begin_class + '0'))  + QString("-") + QString((char)(end_class + '0')) + QString("节"));

    });
}

editclasswidget::~editclasswidget()
{
    delete ui;
}

void editclasswidget::on_change_week_style_clicked()
{
    chooseweek.show();
}


void editclasswidget::on_edit_class_name_line_textChanged(const QString &arg1)
{
    classname = arg1;
}




void editclasswidget::on_extro_info_editline_textChanged(const QString &arg1)
{
    classinfo = arg1;
}


void editclasswidget::on_change_time_style_clicked()
{
    q.show();
}


void editclasswidget::on_changecolorbutton_clicked()
{

    std::vector<int>bubu = generateRichColor();
    ui->pushButton_5->setStyleSheet(
        "background-color: transparent;"
        "color:rgb(" + QString::number(bubu[0]) + "," + QString::number(bubu[1]) + "," + QString::number(bubu[2]) + ");"
        );
    thecolor = QColor(bubu[0], bubu[1] , bubu[2]);
}


void editclasswidget::on_pushButton_51_clicked()
{
    clear();
    this->hide();
    emit data_canceled();

}


void editclasswidget::on_saveButton_clicked()
{
    if(classname != QString(""))
    {
        a_class miaomiao;
        miaomiao.classname = classname;
        miaomiao.classinfo = classinfo;
        miaomiao.beginclass = begin_class;
        miaomiao.endclass = end_class;
        miaomiao.use_precise_time = use_precise_time;
        miaomiao.begintime = begintime;
        miaomiao.endtime = endtime;
        miaomiao.thecolor = thecolor;
        miaomiao.weekinfo = weekinfo;
        miaomiao.which_weekday = which_weekday;
        QTime current = QTime::currentTime();
        miaomiao.classid = current.toString("hhmmss") + QDate::currentDate().toString("MMdd");
       // qDebug() << "miaomiao id" << classid;
        emit data_saved(&miaomiao);
        this->hide();
    }
    else {
        // 显示警告窗口
        QMessageBox::warning(this, "警告", "请输入课程名称！否则无法保存");
        // 防止窗口关闭
        //this->setModal(true);

    }
}

void editclasswidget::on_delete_class_clicked()
{
    a_class miaomiao;
    miaomiao.classname = QString("***###***");
    emit data_saved(&miaomiao);
    this->hide();
}

void editclasswidget::clear()
{
    ui->edit_class_name_line->clear();
    ui->extro_info_editline->clear();
    ui->change_week_style->setText(QString("每周"));
    ui->change_time_style->setText(QString("周一, 第1-1节"));
    chooseweek.clear_redraw();
    q.clear_redraw();
    classname.clear();
    classinfo.clear();
    classid.clear();
    begin_class = 1;
    end_class = 1;
    use_precise_time = false;
    is_hide_delete = true;
    redraw();
    std::vector<int>bubu = generateRichColor();
    ui->pushButton_5->setStyleSheet(
        "background-color: transparent;"
        "color:rgb(" + QString::number(bubu[0]) + "," + QString::number(bubu[1]) + "," + QString::number(bubu[2]) + ");"
        );
    thecolor = QColor(bubu[0], bubu[1] , bubu[2]);
    for(int i = 0; i < 20; i++)
    {
        weekinfo[i] = true;
    }
    which_weekday = 1;
}

void editclasswidget::redraw()
{
    ui->edit_class_name_line->setText(classname);
    ui->extro_info_editline->setText(classinfo);
    //qDebug() << weekinfo.size();
    bool isodd = true;
    bool iseven = true;
    for(int i = 0; i < 20; i++)
    {
        if(this->weekinfo[i] == true && i % 2 == 0)
        {
            iseven = false;
        }
        if(this->weekinfo[i] == true && i % 2 == 1)
        {
            isodd = false;
        }
    }
    if(iseven && !isodd)
    {
        ui->change_week_style->setText(QString("双周"));
    }
    else if(isodd && !iseven){
        ui->change_week_style->setText(QString("单周"));
    }
    else if(!isodd && !iseven)
    {
        ui->change_week_style->setText(QString("每周/自定义"));
    }
    //qDebug() << "now";

    ui->change_time_style->setText(weekday[which_weekday - 1] + QString(" 第") + QString((char)(begin_class + '0'))  + QString("-") + QString((char)(end_class + '0')) + QString("节"));
    ui->pushButton_5->setStyleSheet(
        "background-color: transparent;"
        "color:rgb(" + QString::number(thecolor.red()) + "," + QString::number(thecolor.green()) + "," + QString::number(thecolor.blue()) + ");"
        );

    if(is_hide_delete)
    {
        ui->delete_class->setVisible(false);
    }
    else {
        ui->delete_class->setVisible(true);
    }
}


void editclasswidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //设置背景色（你可以换成别的颜色）
    QColor bgColor = QColor(20, 20, 20,250);
    painter.setBrush(bgColor);
    painter.setPen(Qt::NoPen);

    QRect rect = this->rect();
    painter.drawRoundedRect(rect, 10, 10);  // 10 是圆角半径
}




