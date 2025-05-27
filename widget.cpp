#include "widget.h"
#include "ui_widget.h"
#include <QGraphicsColorizeEffect>
#include <qmetaobject.h>
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QStatusBar>
#include <QApplication>
#include <QPropertyAnimation>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QDate>
#include <QJsonObject>
#include <QDir>
#include "datecalendar.h"
#include "custombutton.h"
#include "smallwidget.h"
#include "smallcustombutton.h"
#include "filehandler.h"
#include "tomatoclock.h"
#include <QGraphicsDropShadowEffect>
#include <QButtonGroup>
#include <weatherfetcher.h>
#include <linearprogressbar.h>
#include "editclasswidget.h"
#include "deepseekclient.h"
#include <QPair>
#include <QTextDocumentFragment>

QString circletype_name[4] = {QString("天"), QString("周"), QString("月"), QString("年")};

void Widget::draw_class_buttons_from_vector()
{
    for(int i =created_class_buttons->size() - 1; i >= 0 ; i--)
    {
        (*created_class_buttons)[i]->redraw();
        if((*created_class_buttons)[i]->weekinfo[selected_weekNumber - 1] == true)
        {
            (*created_class_buttons)[i]->setVisible(true);
            (*created_class_buttons)[i]->is_visi = true;
        }
        else {
            (*created_class_buttons)[i]->setVisible(false);
            (*created_class_buttons)[i]->is_visi = false;
        }
    }

    if(created_class_buttons->size() == 0)
    {
        ui->class_empty_widget->setVisible(true);
    }
    else {
        ui->class_empty_widget->setVisible(false);
    }


    if(selected_weekNumber == 1)
    {
        ui->pushButton_3->setVisible(false);
        ui->pushButton_2->setVisible(true);
    }
    else if(selected_weekNumber == 20)
    {
        ui->pushButton_3->setVisible(true);
        ui->pushButton_2->setVisible(false);
    }
    else {
        ui->pushButton_2->setVisible(true);
        ui->pushButton_3->setVisible(true);
    }
}


void Widget::edit_class(int i)
{

}



void Widget::draw_todo_buttons_from_vector()
{
    // 获取或创建内容控件
    QWidget* contentWidget = ui->TodoList->widget();
    if (!contentWidget) {
        contentWidget = new QWidget(ui->TodoList);
        ui->TodoList->setWidget(contentWidget);
    }

    // 安全清理旧布局
    if (QLayout* oldLayout = contentWidget->layout()) {
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0))) {
            if (QWidget* widget = item->widget()) {
                widget->hide();
                widget->setParent(nullptr);
            }
            delete item;
        }
        delete oldLayout;
    }

    if(created_large_buttons->size() == 0)
    {
        ui->empty_widget->setVisible(true);
    }
    else {
        ui->empty_widget->setVisible(false);
    }



    // 使用垂直布局替代网格布局
    QVBoxLayout* layout = new QVBoxLayout(contentWidget);
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setAlignment(Qt::AlignTop);  // 关键设置：顶部对齐

    // 添加按钮
    for (auto btn : *created_large_buttons) {
        if (btn && !btn->parent() &&!btn->isfinished) {
            btn->setParent(contentWidget);
            btn->show();
            layout->addWidget(btn);
        }
    }

    for(auto btn : *created_large_buttons)
    {
        if (btn && !btn->parent() &&btn->isfinished) {
            btn->setParent(contentWidget);
            btn->show();
            layout->addWidget(btn);
        }
    }

    // 在最后添加拉伸项（保持按钮在顶部）
    layout->addStretch();

    // 调整尺寸
    contentWidget->adjustSize();
    ui->TodoList->updateGeometry();
}

QString Widget::getDataPath(QString filename) {
    // 获取标准路径
    QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    // 确保目录存在
    QDir dir(appDataDir);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qFatal("无法创建应用数据目录: %s", qUtf8Printable(appDataDir));
            return QString();
        }
    }

    // 使用平台无关的路径拼接
    QString ret=QDir::cleanPath(appDataDir + QDir::separator() + filename);
    qDebug()<<ret;
    return ret;
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    QDate ttoday = QDate::currentDate();
    QDate ttomorrow = ttoday.addDays(1);

    QDateTime nowtime = QDateTime::currentDateTime();
    QDateTime tomorrowtime = nowtime.addDays(1);


    ui->setupUi(this);

    ui->now_progres_line->setProgress(0);



    // 获取当前日期
    QDate currentDate = QDate::currentDate();

    // 获取当前日期是周几（Qt: Monday=1, Sunday=7）
    int dayOfWeek = currentDate.dayOfWeek();

    // 计算本周的周一和周日
    QDate monday = currentDate.addDays(1 - dayOfWeek);
    QDate sunday = currentDate.addDays(7 - dayOfWeek);

    // 格式化为 MM.dd - MM.dd（换行）
    QString weekRange = QString("%1 - %2\n")
                            .arg(monday.toString("MM.dd"))
                            .arg(sunday.toString("MM.dd"));

    // 计算第几周
    int days = semesterStart.daysTo(monday);
    selected_weekNumber = days / 7 + 1;

    // 拼接完整文本
    QString labelText = weekRange + QString("   第 %1 周").arg(selected_weekNumber);
    qDebug() << "begin";
     loadTodosFromFile();
    loadClassesFromFile();
    DATA_PATH=getDataPath("todos.json");

    qDebug() << "size " << created_large_buttons->size();

    ui->stackedWidget->setCurrentIndex(0);



    //调节窗口外观
    e.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    e.setAttribute(Qt::WA_TranslucentBackground);  // 允许透明背景
    e.setWindowOpacity(1.0);  // 半透明窗口
    //e.setWindowModality(Qt::ApplicationModal);
    realtime.setInterval(1000);

    connect(&realtime, &QTimer::timeout, this,  &Widget::updateTime);



    ui->chat_result->setStyleSheet(R"(
    QTextBrowser {
        background-color: transparent;
        color: #cccccc;
        border: none;
    }
    QScrollBar:vertical {
        background: #121212;
        width: 8px;
    }
    QScrollBar::handle:vertical {
        background: #555;
        min-height: 20px;
        border-radius: 4px;
    }
)");

    QDate ttttoday = QDate::currentDate();
    QVector<QString> weekDays = { "一", "二", "三", "四", "五", "六","日"};
    ui->MydayDateLabel->setText(ttttoday.toString("MM月dd日") + " 星期" + weekDays[ttttoday.dayOfWeek() - 1]);






    ui->maincontextphoto->setWindowOpacity(0.5);
    QGraphicsColorizeEffect *colorizeEffect = new QGraphicsColorizeEffect;
    colorizeEffect->setColor(Qt::black);
    colorizeEffect->setStrength(0.2);
    ui->maincontextphoto->setGraphicsEffect(colorizeEffect);
    this->setFixedSize(QSize(1033,614));
    ui->TodoList->setStyleSheet("background-color:transparent");
    ui->TododatesetButton->setVisible(false);
    ui->TodotimesetButton->setVisible(false);
    ui->TodorepeatButton->setVisible(false);
    ui->infolabel->setVisible(false);
    ui->repeatinfolabel->setVisible(false);
    ui->edit_widget->setVisible(false);
    ui->addtodo->setTextMargins(10, 0, 90, 0);


    ui->small_tomato_clock_button->setVisible(false);
    ui->tomato_clock_reset_button->setVisible(false);
    ui->tomato_clock_stop_button->setVisible(false);
    ui->label_8->setText("第" + QString::number(fanqie_num) + "个番茄钟");





    // 设置到界面 label
    ui->nowdate_week->setText(labelText);

    ui->dateEdit->setMaximumDate(currentDate.addDays(-1));
    ui->back_to_home_week->setVisible(false);


    QString weatherinfo = WeatherFetcher().getWeatherString("Beijing");
    QString weathertext;
    QString weathericon;
    for(int i = 0; i < 2; i++)
    {
        weathericon += weatherinfo[i];
    }

    QString temperature;

    int weatherIndex = weatherinfo.indexOf("天气：");
    int tempIndex = weatherinfo.indexOf("温度：");

    if (weatherIndex != -1 && tempIndex != -1 && tempIndex > weatherIndex) {
        weathertext = weatherinfo.mid(weatherIndex + 3, tempIndex - (weatherIndex + 3)).trimmed();
        temperature = weatherinfo.mid(tempIndex + 3).trimmed();
    }

    // 设置 UI
    ui->weather_icon->setText(weathericon);  // 这里你没说怎么提取图标，如果有图标符号可以单独提取
    ui->WeatherLabel->setText(weathertext + "\n" + temperature);

    ui->city_name->setText(QString("北京"));




    QButtonGroup * buttonGroupp = new QButtonGroup(this);
    buttonGroupp->setExclusive(true);

    for (int i = 0; i < ui->setting_grid_layout->count(); i++)
    {
        QWidget *widget = ui->setting_grid_layout->itemAt(i)->widget();
        if (QPushButton *btn = qobject_cast<QPushButton*>(widget)) {
            buttonGroupp->addButton(btn, i); // 添加到组中，i 是 id（可选）
        }
    }
    background_ID = 2;
    connect(buttonGroupp, &QButtonGroup::idClicked, this, [=](int id){
        change_background(id);background_ID = id;
    });


    connect(&e , &editclasswidget::data_canceled, this, [this]()
            {
                e.clear();
                e.hide();
                now_edit_class = -1;
            });

    connect(&e, &editclasswidget::data_saved, [this](a_class * temp)
            {
                if(now_edit_class == -1)   // 是添加新课程
                {
                    //qDebug() << "new class";
                    edit_class_button * new_class = new edit_class_button(temp->classid, ui->class_sheet_widget);
                    this->created_class_buttons->push_back(new_class);
                    new_class->setParent(ui->class_sheet_widget);
                    new_class->classname = temp->classname;
                    new_class->classinfo = temp->classinfo;
                    new_class->thecolor = temp->thecolor;
                    //qDebug() << temp->classid << "id" ;
                    new_class->which_weekday = temp->which_weekday;
                    new_class->use_precise_time = temp->use_precise_time;
                    new_class->begintime = temp->begintime;
                    new_class->endtime = temp->endtime;
                    new_class->beginclass = temp->beginclass;
                    new_class->endclass = temp->endclass;
                    new_class->weekinfo = temp->weekinfo;
                    new_class->is_visi = true;
                    new_class->classid = temp->classid;
                    //new_class->redraw();
                    //new_class->show();
                    draw_class_buttons_from_vector();
                    //ui->class_sheet_widget->update();

                    connect(new_class, &edit_class_button::mainbuttonclicked, this, &Widget::open_class_edit_menu);

                }
                else             // 修改既有的课程
                {
                    if(temp->classname == QString("***###***"))
                    {
                        //qDebug() << "heire";
                        (*created_class_buttons)[now_edit_class]->setVisible(false);
                        (*created_class_buttons)[now_edit_class]->is_visi = false;
                        (*created_class_buttons)[now_edit_class]->deleteLater();
                        this->created_class_buttons->remove(now_edit_class);
                        draw_class_buttons_from_vector();
                        now_edit_class = -1;

                    }
                    else
                    {
                        qDebug() << "edit class" << temp->classname;
                         for(int i = 0; i < created_class_buttons->size(); i++)
                        {
                            if((*created_class_buttons)[i]->classid == temp->classid)
                            {
                                now_edit_class = i;
                                break;
                            }
                        }
                        (*created_class_buttons)[now_edit_class]->classname = temp->classname;
                        (*created_class_buttons)[now_edit_class]->classinfo = temp->classinfo;
                        (*created_class_buttons)[now_edit_class]->thecolor = temp->thecolor;
                        (*created_class_buttons)[now_edit_class]->which_weekday = temp->which_weekday;
                        (*created_class_buttons)[now_edit_class]->use_precise_time = temp->use_precise_time;
                        (*created_class_buttons)[now_edit_class]->begintime = temp->begintime;
                        (*created_class_buttons)[now_edit_class]->endtime = temp->endtime;
                        (*created_class_buttons)[now_edit_class]->beginclass = temp->beginclass;
                        (*created_class_buttons)[now_edit_class]->endclass = temp->endclass;
                        (*created_class_buttons)[now_edit_class]->weekinfo = temp->weekinfo;
                        (*created_class_buttons)[now_edit_class]->is_visi = true;
                        (*created_class_buttons)[now_edit_class]->redraw();

                        draw_class_buttons_from_vector();
                        now_edit_class = -1;
                    }
                }
                saveTodosToFile();
                saveClassesToFile();
            });

    //从vector中找到todo并绘制
    draw_todo_buttons_from_vector();




    connect(ui->MydayButton, &QPushButton::clicked, this, [this]()
            {
                ui->stackedWidget->setCurrentIndex(3);
            });
    ui->maincontextphoto_2->setWindowOpacity(0.5);
    ui->maincontextphoto_2->setGraphicsEffect(colorizeEffect);
    connect(ui->ClassesButton, &QPushButton::clicked, this, [this]()
            {
                ui->stackedWidget->setCurrentIndex(1);
            });
    connect(ui->TaskButton, &QPushButton::clicked, this, &Widget::showTomatoClock);

    connect(ui->addtodo, &QLineEdit::textChanged, this, &Widget::updateButtonVisibility);

    QMenu * todo_date_menu = new QMenu(ui->TododatesetButton);

    QAction * today = todo_date_menu->addAction("今天         " + ttoday.toString("MM-dd"));
    QAction * tomorrow = todo_date_menu->addAction("明天         " + ttomorrow.toString("MM-dd"));
    QAction * choose_date = todo_date_menu->addAction("选择日期");
    ui->TododatesetButton->setMenu(todo_date_menu);
    connect(today, &QAction::triggered, this, &Widget::onTodayActionTriggered);
    connect(tomorrow, &QAction::triggered, this, &Widget::onTomorrowActionTriggered);
    connect(choose_date, &QAction::triggered, this, &Widget::onChooseDateActionTriggered);

    ui->TododatesetButton->setPopupMode(QToolButton::InstantPopup);

    todo_date_menu->setStyleSheet(
        "QMenu {"
        "background-color: rgba(20,20,20,200);"  // 菜单背景色
        "border: 0px solid #c0c0c0;"  // 菜单边框
        "margin: 0px;"                // 菜单边距
        "border-radius: 5px;"       // 圆角半径
        "padding: 0px;"               // 菜单内边距
        "}"
        "QMenu::item {"
        "padding: 5px 25px 5px 25px;"  // 菜单项内边距
        "color: white;"               // 菜单项文字颜色
        "min-width: 150px;"          // 菜单项最小宽度
        "max-width: 200px;"          // 菜单项最大宽度
        "min-height: 40px;"
        "max-height: 40px;"
        "}"
        "QMenu::item:selected {"
        "background-color: rgba(80,80,80,255);;"  // 菜单项选中时的背景色
        "}"
        );

    QMenu * todo_time_menu = new QMenu(ui->TodotimesetButton);
    nowtime = nowtime.addSecs(7200);

    QAction *latter_today = todo_time_menu->addAction("今日晚些时候                 " + nowtime.toString("hh:00"));
    QAction *tomorrow_time = todo_time_menu->addAction("明天                             " + tomorrowtime.toString("hh:00"));
    QAction *choose_date_time = todo_time_menu->addAction("选择日期和时间");
    ui->TodotimesetButton->setMenu(todo_time_menu);
    ui->pushButton_15->setEnabled(false);
    connect(latter_today, &QAction::triggered, this, &Widget::onTodaytimeActionTriggered);
    connect(tomorrow_time, &QAction::triggered, this, &Widget::onTomorrowtimeActionTriggered);
    connect(choose_date_time, &QAction::triggered, this, &Widget::onChooseDatetimeActionTriggered);
    todo_time_menu->setStyleSheet( "QMenu {"
                                  "background-color: rgba(20,20,20,200);"  // 菜单背景色
                                  "border: 0px solid #c0c0c0;"  // 菜单边框
                                  "margin: 0px;"                // 菜单边距
                                  "border-radius: 5px;"       // 圆角半径
                                  "padding: 0px;"               // 菜单内边距
                                  "}"
                                  "QMenu::item {"
                                  "padding: 5px 25px 5px 25px;"  // 菜单项内边距
                                  "color: white;"               // 菜单项文字颜色
                                  "min-width: 150px;"          // 菜单项最小宽度
                                  "max-width: 200px;"          // 菜单项最大宽度
                                  "min-height: 40px;"
                                  "max-height: 40px;"
                                  "}"
                                  "QMenu::item:selected {"
                                  "background-color: rgba(80,80,80,255);;"  // 菜单项选中时的背景色
                                  "}");

    QMenu * todo_repeat_menu = new QMenu(ui->TodorepeatButton);
    QAction *every_day = todo_repeat_menu->addAction("每天");
    QAction *every_week = todo_repeat_menu->addAction("每周");
    QAction *set_by_user = todo_repeat_menu->addAction("自定义");
    ui->TodorepeatButton->setMenu(todo_repeat_menu);
    connect(every_day, &QAction::triggered, this, &Widget::onEverydayActionTriggered);
    connect(every_week, &QAction::triggered, this, &Widget::onEveryweekActionTriggered);
    connect(set_by_user, &QAction::triggered, this, &Widget::onSelfdefineActionTriggered);
    todo_repeat_menu->setStyleSheet(        "QMenu {"
                                    "background-color: rgba(20,20,20,200);"  // 菜单背景色
                                    "border: 0px solid #c0c0c0;"  // 菜单边框
                                    "margin: 0px;"                // 菜单边距
                                    "border-radius: 5px;"       // 圆角半径
                                    "padding: 0px;"               // 菜单内边距
                                    "}"
                                    "QMenu::item {"
                                    "padding: 5px 25px 5px 25px;"  // 菜单项内边距
                                    "color: white;"               // 菜单项文字颜色
                                    "min-width: 150px;"          // 菜单项最小宽度
                                    "max-width: 200px;"          // 菜单项最大宽度
                                    "min-height: 40px;"
                                    "max-height: 40px;"
                                    "}"
                                    "QMenu::item:selected {"
                                    "background-color: rgba(80,80,80,255);;"  // 菜单项选中时的背景色
                                    "}");
    qDebug() << "now!\n";


        //update_deepseek_suggestion();

};


void Widget::loadTodosFromFile() {
    QJsonObject root = FileHandler::loadFromFile(DATA_PATH);
    //QJsonArray todosArray = FileHandler::loadFromFile(DATA_PATH);
    this->background_ID = root["id"].toInt();  // 读取全局id变量
    QJsonArray todosArray = root["todos"].toArray();
    //改变背景
    change_background(background_ID);
    foreach (const QJsonValue &value, todosArray) {

        QJsonObject obj = value.toObject();

        // 创建大按钮
        CustomButton* pBtn = new CustomButton(
            obj["title"].toString(),
            obj["id"].toString()
            );
        pBtn->is_have_info = obj["has_info"].toBool();
        pBtn->info = obj["info"].toString();
        pBtn->isfinished = obj["is_finished"].toBool();
        pBtn->is_have_deadline = obj["has_deadline"].toBool();
        pBtn->deadline = QDate::fromString(obj["deadline"].toString(), Qt::ISODate);
        pBtn->is_have_deadtime = obj["has_deadtime"].toBool();
        pBtn->deadtime = QTime::fromString(obj["deadtime"].toString(), Qt::ISODate);
        pBtn->is_have_cycle = obj["has_cycle"].toBool();
        pBtn->cycleline = obj["cycle_info"].toString();
        pBtn->pair.first = obj["unitnum"].toInt();
        pBtn->pair.second = obj["unittype"].toInt();

        pBtn->redraw();
        connect(pBtn, &CustomButton::editButtonClicked, this, &Widget::open_edit_menu);
        connect(pBtn, &CustomButton::mainButtonClicked, this, &Widget::mark_finished);
        connect(pBtn, &CustomButton::deleteButttonClicked, this, &Widget::mark_deleted);
        created_large_buttons->push_back(pBtn);

        //qDebug() << "read " << (*created_large_buttons)[0]->title;

        // 创建小按钮
        SmallCustomButton* spBtn = new SmallCustomButton(
            obj["title"].toString(),
            obj["id"].toString()
            );
        spBtn->is_have_info = obj["has_info"].toBool();
        spBtn->info = obj["info"].toString();
        spBtn->isfinished = obj["is_finished"].toBool();
        spBtn->is_have_deadline = obj["has_deadline"].toBool();
        spBtn->deadline = QDate::fromString(obj["deadline"].toString(), Qt::ISODate);
        spBtn->is_have_deadtime = obj["has_deadtime"].toBool();
        spBtn->deadtime = QTime::fromString(obj["deadtime"].toString(), Qt::ISODate);
        spBtn->is_have_cycle = obj["has_cycle"].toBool();
        spBtn->cycleline = obj["cycle_info"].toString();
        spBtn->pair.first = obj["unitnum"].toInt();
        spBtn->pair.second = obj["unittype"].toInt();

        spBtn->redraw();
        connect(spBtn, &SmallCustomButton::editButtonClicked, this->po, &smallwidget::open_edit_menu);
        connect(spBtn, &SmallCustomButton::mainButtonClicked, this->po, &smallwidget::mark_finished);
        connect(spBtn, &SmallCustomButton::deleteButttonClicked, this->po, &smallwidget::mark_deleted);
        created_small_buttons->push_back(spBtn);

        draw_todo_buttons_from_vector();
    }


    qDebug() << "load size " << created_large_buttons->size();

}

void Widget::saveTodosToFile() {
    QJsonArray todosArray;

    foreach (const CustomButton* btn, *created_large_buttons)
    {
        QJsonObject obj;
        obj["id"] = btn->id;
        obj["title"] = btn->title;
        obj["info"] = btn->info;
        obj["is_finished"] = btn->isfinished;
        obj["has_deadline"] = btn->is_have_deadline;
        obj["deadline"] = btn->deadline.toString(Qt::ISODate);
        obj["has_deadtime"] = btn->is_have_deadtime;
        obj["deadtime"] = btn->deadtime.toString(Qt::ISODate);
        obj["has_cycle"] = btn->is_have_cycle;
        obj["cycle_info"] = btn->cycleline;
        obj["has_info"] = btn->is_have_info;
        obj["unitnum"] = btn->pair.first;
        obj["unittype"] = btn->pair.second;
        todosArray.append(obj);
    }
    QJsonObject root;
    root["id"] = this->background_ID;
    root["todos"] = todosArray;
    if (!FileHandler::saveToFile(DATA_PATH, root)) {
        QMessageBox::warning(this, "错误", "无法保存数据到文件");
    }
}

void Widget::saveClassesToFile(){
    QString filename=getDataPath("classes_info.json");
    QJsonArray jsonArray;
    for(auto it=created_class_buttons->begin();it!=created_class_buttons->end();++it){
        jsonArray.append((*it)->toJson());
    }
    QJsonDocument doc(jsonArray);
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "error" << file.errorString();
        return;
    }
    file.write(doc.toJson(QJsonDocument::Indented));
    return;
}

void Widget::loadClassesFromFile(){
    QString filename=getDataPath("classes_info.json");
    QFile file(filename);
    // 以只读模式打开文件，如果文件不存在则创建
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件:" << file.errorString();
        return;
    }
    QByteArray data=file.readAll();
    QJsonDocument doc=QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isArray()) {
        qWarning() << "JSON 格式错误或根不是数组！";
        return;
    }
    QJsonArray jsonArray = doc.array();
    for (const QJsonValue& value : jsonArray) {
        if (value.isObject()) {
            edit_class_button * new_class = new edit_class_button("classid", ui->class_sheet_widget);
            created_class_buttons->push_back(new_class);
            new_class->fromJson(value.toObject());
            new_class->is_visi = true;
            //new_class->print();
            //new_class->redraw();
            //new_class->show();
            draw_class_buttons_from_vector();
            //ui->class_sheet_widget->update();
            connect(new_class, &edit_class_button::mainbuttonclicked, this, &Widget::open_class_edit_menu);

        } else {
            qWarning() << "发现非对象元素，已跳过";
        }
    }
    return;
}

void Widget::onTodayActionTriggered()
{
    is_todo_date_changed = true;
    todo_set_date = QDate::currentDate();
    ui->infolabel->setVisible(true);
    ui->TododatesetButton->setVisible(false);
    ui->TodotimesetButton->setVisible(false);
    line = todo_set_date.toString("yyyy-MM-dd");
    ui->infolabel->setText(line);
    ui->addtodo->setTextMargins(10, 0 ,200, 0);
}

void Widget::onTomorrowActionTriggered()
{
    is_todo_date_changed = true;
    todo_set_date = QDate::currentDate();
    todo_set_date = todo_set_date.addDays(1);
    ui->infolabel->setVisible(true);
    ui->TododatesetButton->setVisible(false);
    ui->TodotimesetButton->setVisible(false);
    line = todo_set_date.toString("yyyy-MM-dd");
    ui->infolabel->setText(line);
    ui->addtodo->setTextMargins(10, 0 ,200, 0);
}

void Widget::onChooseDateActionTriggered()
{

    w.show();
    w.isvi = false;
    QDate s = QDate::currentDate();
    w.draw_calendar(s);
    connect(&w, &datecalendar::data_saved, [this](QDate &selected_date)
            {
                todo_set_date = selected_date;
                qDebug()<< selected_date.toString("yyyy-MM-dd");
                is_todo_date_changed = true;
                ui->infolabel->setVisible(true);
                ui->TododatesetButton->setVisible(false);
                ui->TodotimesetButton->setVisible(false);
                line = todo_set_date.toString("yyyy-MM-dd");
                ui->infolabel->setText(line);
                ui->addtodo->setTextMargins(10, 0 ,200, 0);
            });
}
void Widget::onTodaytimeActionTriggered()
{
    is_todo_time_changed = true;
    todo_set_time = QTime::currentTime();
    todo_set_time =  todo_set_time.addSecs(7200);
    todo_set_time = QTime(todo_set_time.hour(), 0, 0, 0);
    ui->TododatesetButton->setVisible(false);
    ui->TodotimesetButton->setVisible(false);
    ui->infolabel->setVisible(true);
    line = todo_set_date.toString("yyyy-MM-dd") + QString("\n");
    line += todo_set_time.toString("hh:00");
    ui->infolabel->setText(line);
    ui->addtodo->setTextMargins(10, 0 ,200, 0);
}

void Widget::onTomorrowtimeActionTriggered()
{
    is_todo_time_changed = true;
    is_todo_date_changed = true;
    todo_set_time = QTime::currentTime();
    todo_set_time = QTime(todo_set_time.hour(), 0, 0, 0);
    todo_set_date = todo_set_date.addDays(1);
    ui->TodotimesetButton->setVisible(false);
    ui->TododatesetButton->setVisible(false);
    ui->infolabel->setVisible(true);
    line = todo_set_date.toString("yyyy-MM-dd") + QString("\n");
    line += todo_set_time.toString("hh:00");
    ui->infolabel->setText(line);
    ui->addtodo->setTextMargins(10, 0 ,200, 0);

}

void Widget::onChooseDatetimeActionTriggered()
{
    is_todo_time_changed = true;
    w.show();
    w.isvi = true;
    QDate s = QDate::currentDate();
    w.draw_calendar(s);

    connect(&w, &datecalendar::datas_saved, [this](QPair<QDate, QTime> & datas)
            {
                todo_set_date = datas.first;
                todo_set_time = datas.second;
                qDebug() << todo_set_date.toString("yyyy-MM-dd") << "    " << todo_set_time.toString("hh-mm-ss");
                ui->TododatesetButton->setVisible(false);
                ui->TodotimesetButton->setVisible(false);
                ui->infolabel->setVisible(true);
                line = todo_set_date.toString("yyyy-MM-dd") + QString("\n") + todo_set_time.toString("hh:00");
                ui->infolabel->setText(line);
                ui->addtodo->setTextMargins(10, 0 ,200, 0);
            });
}

void Widget::onEverydayActionTriggered()
{
    ui->TodorepeatButton->setVisible(false);
    unitnum = 1;
    unittype = 0;
    ui->repeatinfolabel->setVisible(true);
    repeat_info = QString("每日1次");
    ui->repeatinfolabel->setText(repeat_info);
    ui->addtodo->setTextMargins(10, 0 ,200, 0);
}

void Widget::onEveryweekActionTriggered()
{
    unitnum = 1;
    unittype = 1;
    ui->TodorepeatButton->setVisible(false);
    ui->repeatinfolabel->setVisible(true);
    repeat_info = QString("每周1次");
    ui->repeatinfolabel->setText(repeat_info);
    ui->addtodo->setTextMargins(10, 0 ,200, 0);
}

void Widget::onSelfdefineActionTriggered()
{
    c.show();
    connect(&c, &subrepeat::datas_save, [this](QPair<int, int> & datas)
            {
                unitnum = datas.first;
                unittype = datas.second;
                qDebug() << unitnum<< "    " << unittype;
                ui->TodorepeatButton->setVisible(false);
                ui->repeatinfolabel->setVisible(true);
                if(unitnum <= 0 || unittype < 0)
                {
                    repeat_info = QString("无循环");
                }
                else {
                    repeat_info = QString("每") + QString((char)('0' + unitnum)) + circletype_name[unittype] + QString("1次");
                }

                ui->repeatinfolabel->setText(repeat_info);
                ui->addtodo->setTextMargins(10, 0 ,200, 0);
            });
}



void Widget::updateButtonVisibility(const QString &text) //相当于初始化所有的todo信息
{
    ui->addtodo->setTextMargins(10, 0, 90, 0);
    ui->infolabel->setVisible(false);
    ui->repeatinfolabel->setVisible(false);
    ui->TododatesetButton->setVisible(!text.isEmpty());
    ui->TodotimesetButton->setVisible(!text.isEmpty());
    ui->TodorepeatButton->setVisible(!text.isEmpty());
    line.clear();
    repeat_info.clear();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_addtodo_returnPressed()
{
    // TODO: 将这个todo添加到后端
    if(!ui->addtodo->text().isEmpty())
    {
        QString todo_title = ui->addtodo->text();
        QTime cur = QTime::currentTime();
        QString id = cur.toString("hhmmss") + todo_title;

        CustomButton* pBtn = new CustomButton(todo_title, id);
        pBtn->title = todo_title;
        pBtn->is_have_deadline = is_todo_date_changed;
        pBtn->is_have_deadtime = is_todo_time_changed;
        pBtn->is_have_cycle = (bool)unitnum;
        pBtn->cycleline = repeat_info;
        pBtn->pair.first = unitnum;
        pBtn->pair.second = unittype;
        pBtn->deadline = todo_set_date;
        pBtn->deadtime = todo_set_time;
        pBtn->redraw();
        connect(pBtn, &CustomButton::editButtonClicked, this, &Widget::open_edit_menu);
        connect(pBtn, &CustomButton::mainButtonClicked, this, &Widget::mark_finished);
        connect(pBtn, &CustomButton::deleteButttonClicked, this, &Widget::mark_deleted);
        created_large_buttons->push_back(pBtn);

        SmallCustomButton* spBtn = new SmallCustomButton(todo_title, id);
        spBtn->title = todo_title;
        spBtn->is_have_deadline = is_todo_date_changed;
        spBtn->is_have_deadtime = is_todo_time_changed;
        spBtn->is_have_cycle = (bool)unitnum;
        spBtn->cycleline = repeat_info;
        spBtn->pair.first = unitnum;
        spBtn->pair.second = unittype;
        spBtn->deadline = todo_set_date;
        spBtn->deadtime = todo_set_time;
        spBtn->redraw();
        created_small_buttons->push_back(spBtn);
        connect(spBtn, &SmallCustomButton::editButtonClicked, this->po, &smallwidget::open_edit_menu);
        connect(spBtn, &SmallCustomButton::mainButtonClicked, this->po, &smallwidget::mark_finished);
        connect(spBtn, &SmallCustomButton::deleteButttonClicked, this->po, &smallwidget::mark_deleted);
        draw_todo_buttons_from_vector();

        is_todo_date_changed = false;
        is_todo_time_changed = false;
        ui->addtodo->clear();
        line.clear();
        repeat_info.clear();
    }
}

void Widget::mark_deleted(const QString &id)
{
    for(int i = 0; i < created_large_buttons->size(); i++)
    {
        if((*created_large_buttons)[i]->id == id)
        {
            now_edit_bbutton = i;
            break;
        }
    }
    created_large_buttons->remove(now_edit_bbutton);
    created_small_buttons->remove(now_edit_bbutton);
    now_edit_bbutton = -1;
    draw_todo_buttons_from_vector();
}


void Widget::mark_finished(const QString &id)
{
    for(int i = 0; i < created_large_buttons->size(); i++)
    {
        if((*created_large_buttons)[i]->id == id)
        {
            now_edit_bbutton = i;
            break;
        }
    }
    (*created_large_buttons)[now_edit_bbutton]->isfinished = true;
    (*created_small_buttons)[now_edit_bbutton]->isfinished = true;
    (*created_large_buttons)[now_edit_bbutton]->redraw();
    (*created_small_buttons)[now_edit_bbutton]->redraw();
    now_edit_bbutton = -1;
    draw_todo_buttons_from_vector();

}


void Widget::open_edit_menu(const QString &id)
{
    for(int i = 0; i < created_large_buttons->size(); i++)
    {
        if((*created_large_buttons)[i]->id == id)
        {
            now_edit_bbutton = i;
            break;
        }
    }
    ui->edit_widget->setVisible(true);
    if(!(*created_large_buttons)[now_edit_bbutton]->info.isEmpty())
    {
        ui->add_todo_info->setText((*created_large_buttons)[now_edit_bbutton]->info);
    }
    else {
        ui->add_todo_info->setText("");
    }
    ui->edit_todo_title->setText((*created_large_buttons)[now_edit_bbutton]->title);

    for(int i = 0; i < (*created_large_buttons).size(); i++)
    {
        (*created_large_buttons)[i]->mainButton->setEnabled(false);
        (*created_large_buttons)[i]->editButton->setEnabled(false);
        (*created_large_buttons)[i]->redraw();
    }
    ui->addtodo->setEnabled(false);



    if((*created_large_buttons)[now_edit_bbutton]->is_have_deadline)
    {
        ui->reset_todo_date->setVisible(true);
        QString textline = (*created_large_buttons)[now_edit_bbutton]->deadline.toString("MM-dd");
        ui->reset_todo_date->setText((QString("  改变截止日期                        ") + textline));
    }
    else {
        ui->reset_todo_date->setText(QString("  改变截止日期     "));
        ui->reset_todo_date->setVisible(true);
    }

    if((*created_large_buttons)[now_edit_bbutton]->is_have_deadtime)
    {
        QString textline = (*created_large_buttons)[now_edit_bbutton]->deadtime.toString(("hh:00"));
        ui->reset_todo_time->setText((QString("  提醒我                                  ") + textline));
        ui->reset_todo_date->setVisible(false);
    }
    else {
        ui->reset_todo_time->setText(QString("  提醒我          "));
    }

    if((*created_large_buttons)[now_edit_bbutton]->is_have_cycle)
    {
        ui->reset_todo_repeat->setText(QString("  重复                               ") + (*created_large_buttons)[now_edit_bbutton]->cycleline);
    }
    else {
        ui->reset_todo_repeat->setText(QString("  重复                               ") + (*created_large_buttons)[now_edit_bbutton]->cycleline);
    }
}


void Widget::open_class_edit_menu(const QString &id)
{
    e.clear();
    qDebug() << "open class edit menu" << id;
    for(int i = 0; i < created_class_buttons->size(); i++)
    {
        if((*created_class_buttons)[i]->classid == id)
        {
            now_edit_class = i;
            break;
        }
    }

    e.classname = (*created_class_buttons)[now_edit_class]->classname;
    e.classinfo = (*created_class_buttons)[now_edit_class]->classinfo;
    e.classid = (*created_class_buttons)[now_edit_class]->classid;
    e.thecolor = (*created_class_buttons)[now_edit_class]->thecolor;
    e.begintime = (*created_class_buttons)[now_edit_class]->begintime;
    e.endtime = (*created_class_buttons)[now_edit_class]->endtime;
    e.begin_class = (*created_class_buttons)[now_edit_class]->beginclass;
    e.end_class = (*created_class_buttons)[now_edit_class]->endclass;
    e.use_precise_time = (*created_class_buttons)[now_edit_class]->use_precise_time;
    e.weekinfo = (*created_class_buttons)[now_edit_class]->weekinfo;
    e.which_weekday = (*created_class_buttons)[now_edit_class]->which_weekday;
    e.is_hide_delete = false;
    e.redraw();
    e.show();
}


void Widget::on_close_edit_button_clicked()
{
    for(int i = 0; i < (*created_large_buttons).size(); i++)
    {
        if(!(*created_large_buttons)[i]->isfinished)
        {
            (*created_large_buttons)[i]->mainButton->setEnabled(true);
            (*created_large_buttons)[i]->editButton->setEnabled(true);
            (*created_large_buttons)[i]->redraw();
        }
    }
    ui->addtodo->setEnabled(true);


    ui->edit_widget->setVisible(false);
    now_edit_bbutton = -1;
    ui->add_todo_info->setVisible(true);
    ui->reset_todo_date->setVisible(true);
    ui->reset_todo_repeat->setVisible(true);
    ui->reset_todo_time->setVisible(true);
    draw_todo_buttons_from_vector();
    is_todo_date_changed = false;
    is_todo_time_changed = false;
    ui->set_todo_finished->setStyleSheet(

        "QPushButton {"
        "background-color: rgba(50, 50, 50, 255);"


        "color: white; /* 文字颜色为白色 */"
        "border: none; /* 去掉边框 */ "
        "border-bottom-left-radius: 10px;"
        "border-top-left-radius: 10px;"
        "icon: url(:/Themes/Assets/circle-large.svg); /* 正常状态图标 */"
        "padding-left: 10px;"
        "text-align: left; /* 文字居左 */"
        "}"

        "QPushButton:hover "
        "{ "
        "background-color: rgba(0, 0, 0, 128); /* 悬浮时背景颜色变深 */ "
        "icon: url(:/Themes/Assets/circle-check.svg); /* 正常状态图标 */ "
        "}");


}


void Widget::on_edit_todo_title_textChanged(const QString &arg1)
{
    (*created_large_buttons)[now_edit_bbutton]->title = arg1;
    (*created_small_buttons)[now_edit_bbutton]->title = arg1;
    (*created_large_buttons)[now_edit_bbutton]->redraw();
    (*created_small_buttons)[now_edit_bbutton]->redraw();
}


void Widget::on_set_todo_finished_clicked()
{
    (*created_large_buttons)[now_edit_bbutton]->isfinished = true;
    (*created_small_buttons)[now_edit_bbutton]->isfinished = true;
    (*created_large_buttons)[now_edit_bbutton]->redraw();
    (*created_small_buttons)[now_edit_bbutton]->redraw();

    ui->set_todo_finished->setStyleSheet(

        "QPushButton {"
        "background-color: rgba(50, 50, 50, 255);"


        "color: white; /* 文字颜色为白色 */"
        "border: none; /* 去掉边框 */ "
        "border-bottom-left-radius: 10px;"
        "border-top-left-radius: 10px;"
        "icon: url(:/Themes/Assets/finish.svg); /* 正常状态图标 */"
        "padding-left: 10px;"
        "text-align: left; /* 文字居左 */"
        "}"

        "QPushButton:hover "
        "{ "
        "background-color: rgba(50, 50, 50, 255); /* 悬浮时背景颜色变深 */ "
        "icon: url(:/Themes/Assets/finish.svg); /* 正常状态图标 */ "
        "}");
    ui->add_todo_info->setVisible(false);
    ui->reset_todo_date->setVisible(false);
    ui->reset_todo_repeat->setVisible(false);
    ui->reset_todo_time->setVisible(false);

    now_edit_bbutton = -1;

}


void Widget::on_add_todo_info_textChanged()
{
    (*created_large_buttons)[now_edit_bbutton]->info = ui->add_todo_info->toPlainText();
    (*created_small_buttons)[now_edit_bbutton]->info = ui->add_todo_info->toPlainText();
    if(!(*created_large_buttons)[now_edit_bbutton]->info.isEmpty())
    {
        (*created_large_buttons)[now_edit_bbutton]->is_have_info = true;
        (*created_small_buttons)[now_edit_bbutton]->is_have_info = true;
        (*created_large_buttons)[now_edit_bbutton]->redraw();
        (*created_small_buttons)[now_edit_bbutton]->redraw();
    }
    else {
        (*created_large_buttons)[now_edit_bbutton]->is_have_info = false;
        (*created_small_buttons)[now_edit_bbutton]->is_have_info = false;
        (*created_large_buttons)[now_edit_bbutton]->redraw();
        (*created_small_buttons)[now_edit_bbutton]->redraw();
    }
}


void Widget::on_reset_todo_time_clicked()
{
    w.show();
    w.isvi = true;
    QDate s = QDate::currentDate();
    w.draw_calendar(s);
    connect(&w, &datecalendar::datas_saved, [this](QPair<QDate, QTime> & datas)
            {
                (*created_large_buttons)[now_edit_bbutton]->deadline = datas.first;
                (*created_small_buttons)[now_edit_bbutton]->deadline = datas.first;
                (*created_large_buttons)[now_edit_bbutton]->deadtime = datas.second;
                (*created_small_buttons)[now_edit_bbutton]->deadtime = datas.second;
                ui->reset_todo_date->setVisible(false);
                line = datas.second.toString("hh:00");
                ui->reset_todo_time->setText(QString("  提醒我                                ") + line);

                (*created_large_buttons)[now_edit_bbutton]->is_have_deadline = true;
                (*created_small_buttons)[now_edit_bbutton]->is_have_deadline = true;
                (*created_large_buttons)[now_edit_bbutton]->is_have_deadtime = true;
                (*created_small_buttons)[now_edit_bbutton]->is_have_deadtime = true;
                (*created_large_buttons)[now_edit_bbutton]->redraw();
                (*created_small_buttons)[now_edit_bbutton]->redraw();
            });

}


void Widget::on_reset_todo_date_clicked()
{
    w.show();
    w.isvi = false;
    QDate s = QDate::currentDate();
    w.draw_calendar(s);
    connect(&w, &datecalendar::data_saved, [this](QDate &selected_date)
            {
                (*created_large_buttons)[now_edit_bbutton]->deadline = selected_date;
                (*created_small_buttons)[now_edit_bbutton]->deadline = selected_date;
                line = selected_date.toString("MM-dd");
                ui->reset_todo_date->setText(QString("  改变截止日期                        ") + line);
                (*created_large_buttons)[now_edit_bbutton]->is_have_deadline = true;
                (*created_small_buttons)[now_edit_bbutton]->is_have_deadline = true;
                (*created_large_buttons)[now_edit_bbutton]->is_have_deadtime = false;
                (*created_small_buttons)[now_edit_bbutton]->is_have_deadtime = false;
                (*created_large_buttons)[now_edit_bbutton]->redraw();
                (*created_small_buttons)[now_edit_bbutton]->redraw();
            });
}


void Widget::on_reset_todo_repeat_clicked()
{
    c.show();
    connect(&c, &subrepeat::datas_save, [this](QPair<int, int> & datas)
            {
                unitnum = datas.first;
                unittype = datas.second;
                if(unitnum <= 0 || unittype < 0)
                {
                    repeat_info = QString("无循环");
                }
                else {
                    repeat_info = QString("每") + QString((char)('0' + unitnum)) + circletype_name[unittype] + QString("1次");
                }
                (*created_large_buttons)[now_edit_bbutton]->cycleline = repeat_info;
                (*created_large_buttons)[now_edit_bbutton]->pair.first = unitnum;
                (*created_large_buttons)[now_edit_bbutton]->pair.second = unittype;
                (*created_large_buttons)[now_edit_bbutton]->is_have_cycle = true;
                (*created_small_buttons)[now_edit_bbutton]->cycleline = repeat_info;
                (*created_small_buttons)[now_edit_bbutton]->pair.first = unitnum;
                (*created_small_buttons)[now_edit_bbutton]->pair.second = unittype;
                (*created_small_buttons)[now_edit_bbutton]->is_have_cycle = true;

                if((*created_small_buttons)[now_edit_bbutton]->is_have_cycle)
                {
                    ui->reset_todo_repeat->setText(QString("  重复                               ") + (*created_small_buttons)[now_edit_bbutton]->cycleline);
                }
                else {
                    ui->reset_todo_repeat->setText(QString("  重复                               ") + (*created_small_buttons)[now_edit_bbutton]->cycleline);
                }
                (*created_small_buttons)[now_edit_bbutton]->redraw();
                (*created_large_buttons)[now_edit_bbutton]->redraw();
            });

}


void Widget::on_delete_todo_clicked()
{
    created_large_buttons->remove(now_edit_bbutton);
    created_small_buttons->remove(now_edit_bbutton);
    ui->edit_widget->setVisible(false);
    draw_todo_buttons_from_vector();
    for(int i = 0; i < (*created_large_buttons).size(); i++)
    {
        if(!(*created_large_buttons)[i]->isfinished)
        {
            (*created_large_buttons)[i]->mainButton->setEnabled(true);
            (*created_large_buttons)[i]->editButton->setEnabled(true);
            (*created_large_buttons)[i]->redraw();
        }
    }
    ui->addtodo->setEnabled(true);
}

void Widget::on_chagesize_clicked()
{
    emit changesizeButtonClicked();
}

void Widget::on_add_class_by_hand_clicked()
{
    e.clear();
    e.redraw();
    e.show();

    // emit changesizeButtonClicked();
}



void Widget::on_add_class_from_web_clicked()
{
#ifdef USE_WEB
    QVector<Savedata> web_data=get_data_from_web();
    //qDebug()<<"get_data_from_web() done";
    int tmp=0;
    for (auto it=web_data.begin();it!=web_data.end();++it,++tmp){
        //it->print();
        QTime current = QTime::currentTime();
        QString classid = current.toString("hhmmss") + QDate::currentDate().toString("MMdd")+"_"+QString::number(tmp);
        edit_class_button * new_class = new edit_class_button(classid, ui->class_sheet_widget);
        qDebug() << classid;
        created_class_buttons->push_back(new_class);
        new_class->setParent(ui->class_sheet_widget);
        new_class->classname = it->classname;
        new_class->classinfo = it->classinfo;
        new_class->thecolor = it->thecolor;
        //qDebug() << temp->classid << "id" ;
        new_class->which_weekday = it->which_weekday;
        new_class->use_precise_time = it->use_precise_time;
        new_class->begintime = it->begintime;
        new_class->endtime = it->endtime;
        new_class->beginclass = it->begin_class;
        new_class->endclass = it->end_class;
        new_class->weekinfo = QVector<bool>(20);
        for(int i=0;i<19;i++){new_class->weekinfo[i]=it->weekinfo[i+1];}
        new_class->weekinfo[19]=false;
        new_class->is_visi = true;
        //new_class->redraw();
        //new_class->show();
        draw_class_buttons_from_vector();
        //ui->class_sheet_widget->update();

        connect(new_class, &edit_class_button::mainbuttonclicked, this, &Widget::open_class_edit_menu);
    }

    saveTodosToFile();
    saveClassesToFile();

    return;
#else
    qDebug()<<"get_data_from_web() called";
#endif

}

void Widget::showTomatoClock()
{

    ui->stackedWidget->setCurrentIndex(4);
}

void Widget::handleTomatoClockClosed()
{
    tomatoWidget = nullptr;
}



void Widget::on_pushButton_2_clicked()
{
    // 增加周数
    selected_weekNumber++;

    // 更新上一周按钮可见性
    ui->pushButton_3->setVisible(true);

    // 如果是第20周，隐藏“下一周”按钮
    if (selected_weekNumber == 20) {
        ui->pushButton_2->setVisible(false);
    }
    QDate monday = semesterStart.addDays((selected_weekNumber - 1) * 7);
    QDate sunday = monday.addDays(6);

    // 格式化输出：MM.dd - MM.dd 换行 第 N 周
    QString labelText = QString("%1 - %2\n   第 %3 周")
                            .arg(monday.toString("MM.dd"))
                            .arg(sunday.toString("MM.dd"))
                            .arg(selected_weekNumber);

    ui->nowdate_week->setText(labelText);

    QDate currentDate = QDate::currentDate();

    // 获取当前日期是周几（Qt: Monday=1, Sunday=7）
    int dayOfWeek = currentDate.dayOfWeek();

    // 计算本周的周一和周日
    QDate mmonday = currentDate.addDays(1 - dayOfWeek);
    QDate ssunday = currentDate.addDays(7 - dayOfWeek);

    // 格式化为 MM.dd - MM.dd（换行）
    QString weekRange = QString("%1 - %2\n")
                            .arg(mmonday.toString("MM.dd"))
                            .arg(ssunday.toString("MM.dd"));

    // 计算第几周
    int days = semesterStart.daysTo(mmonday);
    int sselected_weekNumber = days / 7 + 1;




    if(selected_weekNumber == sselected_weekNumber)
    {
        ui->back_to_home_week->setVisible(false);
    }
    else {
        ui->back_to_home_week->setVisible(true);
    }

    draw_class_buttons_from_vector();
}



void Widget::on_pushButton_3_clicked()
{
    selected_weekNumber--;

    // 到第1周不能再往前，隐藏“上一周”按钮
    if (selected_weekNumber == 1) {
        ui->pushButton_3->setVisible(false);
    }

    // 重新显示“下一周”按钮
    ui->pushButton_2->setVisible(true);

    QDate monday = semesterStart.addDays((selected_weekNumber - 1) * 7);
    QDate sunday = monday.addDays(6);

    QString labelText = QString("%1 - %2\n   第 %3 周")
                            .arg(monday.toString("MM.dd"))
                            .arg(sunday.toString("MM.dd"))
                            .arg(selected_weekNumber);

    ui->nowdate_week->setText(labelText);

    QDate currentDate = QDate::currentDate();

    // 获取当前日期是周几（Qt: Monday=1, Sunday=7）
    int dayOfWeek = currentDate.dayOfWeek();

    // 计算本周的周一和周日
    QDate mmonday = currentDate.addDays(1 - dayOfWeek);
    QDate ssunday = currentDate.addDays(7 - dayOfWeek);

    // 格式化为 MM.dd - MM.dd（换行）
    QString weekRange = QString("%1 - %2\n")
                            .arg(mmonday.toString("MM.dd"))
                            .arg(ssunday.toString("MM.dd"));

    // 计算第几周
    int days = semesterStart.daysTo(mmonday);
    int sselected_weekNumber = days / 7 + 1;




    if(selected_weekNumber == sselected_weekNumber)
    {
        ui->back_to_home_week->setVisible(false);
    }
    else {
        ui->back_to_home_week->setVisible(true);
    }

    draw_class_buttons_from_vector();
}

void Widget::change_background(int id)
{
    if(id == 2)
    {
        ui->setting_photo->setStyleSheet("QWidget#setting_photo"
                                         "{"
                                             "image: url(:/Themes/Assets/Themes/backgrounds/beach/2560x1600.jpg);"

                                         "}"
                                         );
        ui->maincontextphoto->setStyleSheet(
            "QWidget#maincontextphoto"
            "{"
                "image: url(:/Themes/Assets/Themes/backgrounds/beach/2560x1600.jpg);"

            "}"

            );

        ui->maincontextphoto_2->setStyleSheet("QWidget#maincontextphoto_2"
                                              "{"
                                                  "image: url(:/Themes/Assets/Themes/backgrounds/beach/2560x1600.jpg);"

                                              "}");
        ui->tomato_clock_background->setStyleSheet("QWidget#tomato_clock_background"
                                                   "{"
                                                       "image: url(:/Themes/Assets/Themes/backgrounds/beach/2560x1600.jpg);"

                                                  " }");
        ui->deepseek_background->setStyleSheet("QWidget#deepseek_background"
                                               "{"
                                                   "image: url(:/Themes/Assets/Themes/backgrounds/beach/2560x1600.jpg);"

                                               "}");
    }
    else if(id == 4)
    {
        ui->setting_photo->setStyleSheet("QWidget#setting_photo"
                                         "{"
                                         "image: url(:/Themes/Assets/Themes/backgrounds/desert/2560x1600.jpg);"

                                         "}"
                                         );
        ui->maincontextphoto->setStyleSheet(
            "QWidget#maincontextphoto"
            "{"
            "image: url(:/Themes/Assets/Themes/backgrounds/desert/2560x1600.jpg);"

            "}"

            );

        ui->maincontextphoto_2->setStyleSheet("QWidget#maincontextphoto_2"
                                              "{"
                                              "image: url(:/Themes/Assets/Themes/backgrounds/desert/2560x1600.jpg);"

                                              "}");
        ui->tomato_clock_background->setStyleSheet("QWidget#tomato_clock_background"
                                                   "{"
                                                       "image: url(:/Themes/Assets/Themes/backgrounds/desert/2560x1600.jpg);"

                                                  " }");
        ui->deepseek_background->setStyleSheet("QWidget#deepseek_background"
                                               "{"
                                                   "image: url(:/Themes/Assets/Themes/backgrounds/desert/2560x1600.jpg);"

                                               "}");
    }
    else if(id == 3)
    {
        ui->setting_photo->setStyleSheet("QWidget#setting_photo"
                                         "{"
                                         "image: url(:/Themes/Assets/Themes/backgrounds/field/2560x1600.jpg);"

                                         "}"
                                         );
        ui->maincontextphoto->setStyleSheet(
            "QWidget#maincontextphoto"
            "{"
            "image: url(:/Themes/Assets/Themes/backgrounds/field/2560x1600.jpg);"

            "}"

            );

        ui->maincontextphoto_2->setStyleSheet("QWidget#maincontextphoto_2"
                                              "{"
                                              "image: url(:/Themes/Assets/Themes/backgrounds/field/2560x1600.jpg);"

                                              "}");
        ui->tomato_clock_background->setStyleSheet("QWidget#tomato_clock_background"
                                                   "{"
                                                       "image: url(:/Themes/Assets/Themes/backgrounds/field/2560x1600.jpg);"

                                                  " }");
        ui->deepseek_background->setStyleSheet("QWidget#deepseek_background"
                                               "{"
                                                   "image: url(:/Themes/Assets/Themes/backgrounds/field/2560x1600.jpg);"

                                               "}");
    }
    else if(id == 0)
    {
        ui->setting_photo->setStyleSheet("QWidget#setting_photo"
                                         "{"
                                         "image: url(:/Themes/Assets/Themes/backgrounds/fern/2560x1600.jpg);"

                                         "}"
                                         );
        ui->maincontextphoto->setStyleSheet(
            "QWidget#maincontextphoto"
            "{"
            "image: url(:/Themes/Assets/Themes/backgrounds/fern/2560x1600.jpg);"

            "}"

            );

        ui->maincontextphoto_2->setStyleSheet("QWidget#maincontextphoto_2"
                                              "{"
                                              "image: url(:/Themes/Assets/Themes/backgrounds/fern/2560x1600.jpg);"

                                              "}");
        ui->tomato_clock_background->setStyleSheet("QWidget#tomato_clock_background"
                                                   "{"
                                                       "image: url(:/Themes/Assets/Themes/backgrounds/fern/2560x1600.jpg);"

                                                  " }");
        ui->deepseek_background->setStyleSheet("QWidget#deepseek_background"
                                               "{"
                                                   "image: url(:/Themes/Assets/Themes/backgrounds/fern/2560x1600.jpg);"

                                               "}");
    }
    else if(id == 1)
    {
        ui->setting_photo->setStyleSheet("QWidget#setting_photo"
                                         "{"
                                         "image: url(:/Themes/Assets/Themes/backgrounds/gradient/2560x1600.jpg);"

                                         "}"
                                         );
        ui->maincontextphoto->setStyleSheet(
            "QWidget#maincontextphoto"
            "{"
            "image: url(:/Themes/Assets/Themes/backgrounds/gradient/2560x1600.jpg);"

            "}"

            );

        ui->maincontextphoto_2->setStyleSheet("QWidget#maincontextphoto_2"
                                              "{"
                                              "image: url(:/Themes/Assets/Themes/backgrounds/gradient/2560x1600.jpg);"

                                              "}");
        ui->tomato_clock_background->setStyleSheet("QWidget#tomato_clock_background"
                                                   "{"
                                                       "image: url(:/Themes/Assets/Themes/backgrounds/gradient/2560x1600.jpg);"

                                                  " }");
        ui->deepseek_background->setStyleSheet("QWidget#deepseek_background"
                                               "{"
                                                   "image: url(:/Themes/Assets/Themes/backgrounds/gradient/2560x1600.jpg);"

                                               "}");
    }
    else if(id == 5)
    {
        ui->setting_photo->setStyleSheet("QWidget#setting_photo"
                                         "{"
                                         "image: url(:/Themes/Assets/Themes/backgrounds/lighthouse/2560x1600.jpg);"

                                         "}"
                                         );
        ui->maincontextphoto->setStyleSheet(
            "QWidget#maincontextphoto"
            "{"
            "image: url(:/Themes/Assets/Themes/backgrounds/lighthouse/2560x1600.jpg);"

            "}"

            );

        ui->maincontextphoto_2->setStyleSheet("QWidget#maincontextphoto_2"
                                              "{"
                                              "image: url(:/Themes/Assets/Themes/backgrounds/lighthouse/2560x1600.jpg);"

                                              "}");
        ui->tomato_clock_background->setStyleSheet("QWidget#tomato_clock_background"
                                                   "{"
                                                       "image: url(:/Themes/Assets/Themes/backgrounds/lighthouse/2560x1600.jpg);"

                                                  " }");
        ui->deepseek_background->setStyleSheet("QWidget#deepseek_background"
                                               "{"
                                                   "image: url(:/Themes/Assets/Themes/backgrounds/lighthouse/2560x1600.jpg);"

                                               "}");
    }
    else if(id == 6)
    {
        ui->setting_photo->setStyleSheet("QWidget#setting_photo"
                                         "{"
                                         "image: url(:/Themes/Assets/Themes/backgrounds/safari/2560x1600.jpg);"

                                         "}"
                                         );
        ui->maincontextphoto->setStyleSheet(
            "QWidget#maincontextphoto"
            "{"
            "image: url(:/Themes/Assets/Themes/backgrounds/safari/2560x1600.jpg);"

            "}"

            );

        ui->maincontextphoto_2->setStyleSheet("QWidget#maincontextphoto_2"
                                              "{"
                                              "image: url(:/Themes/Assets/Themes/backgrounds/safari/2560x1600.jpg);"

                                              "}");
        ui->tomato_clock_background->setStyleSheet("QWidget#tomato_clock_background"
                                                   "{"
                                                       "image: url(:/Themes/Assets/Themes/backgrounds/safari/2560x1600.jpg);"

                                                  " }");
        ui->deepseek_background->setStyleSheet("QWidget#deepseek_background"
                                               "{"
                                                   "image: url(:/Themes/Assets/Themes/backgrounds/safari/2560x1600.jpg);"

                                               "}");
    }
    else if(id == 7)
    {
        ui->setting_photo->setStyleSheet("QWidget#setting_photo"
                                         "{"
                                         "image: url(:/Themes/Assets/Themes/backgrounds/sea/2560x1600.jpg);"

                                         "}"
                                         );
        ui->maincontextphoto->setStyleSheet(
            "QWidget#maincontextphoto"
            "{"
            "image: url(:/Themes/Assets/Themes/backgrounds/sea/2560x1600.jpg);"

            "}"

            );

        ui->maincontextphoto_2->setStyleSheet("QWidget#maincontextphoto_2"
                                              "{"
                                              "image: url(:/Themes/Assets/Themes/backgrounds/sea/2560x1600.jpg);"

                                              "}");
        ui->tomato_clock_background->setStyleSheet("QWidget#tomato_clock_background"
                                                   "{"
                                                       "image: url(:/Themes/Assets/Themes/backgrounds/sea/2560x1600.jpg);"

                                                  " }");
        ui->deepseek_background->setStyleSheet("QWidget#deepseek_background"
                                               "{"
                                                   "image: url(:/Themes/Assets/Themes/backgrounds/sea/2560x1600.jpg);"

                                               "}");
    }
    else if(id == 8)
    {
        ui->setting_photo->setStyleSheet("QWidget#setting_photo"
                                         "{"
                                         "image: url(:/Themes/Assets/Themes/backgrounds/tv_tower/2560x1600.jpg);"

                                         "}"
                                         );
        ui->maincontextphoto->setStyleSheet(
            "QWidget#maincontextphoto"
            "{"
            "image: url(:/Themes/Assets/Themes/backgrounds/tv_tower/2560x1600.jpg);"

            "}"

            );

        ui->maincontextphoto_2->setStyleSheet("QWidget#maincontextphoto_2"
                                              "{"
                                              "image: url(:/Themes/Assets/Themes/backgrounds/tv_tower/2560x1600.jpg);"

                                              "}");
        ui->tomato_clock_background->setStyleSheet("QWidget#tomato_clock_background"
                                                   "{"
                                                       "image: url(:/Themes/Assets/Themes/backgrounds/tv_tower/2560x1600.jpg);"

                                                  " }");
        ui->deepseek_background->setStyleSheet("QWidget#deepseek_background"
                                               "{"
                                                   "image: url(:/Themes/Assets/Themes/backgrounds/tv_tower/2560x1600.jpg);"

                                               "}");
    }
    else if(id == 9)
    {
        ui->setting_photo->setStyleSheet("QWidget#setting_photo"
                                         "{"
                                         "image: url(:/Themes/Assets/Themes/backgrounds/beach/2560x1600.jpg);"

                                         "}"
                                         );
        ui->maincontextphoto->setStyleSheet(
            "QWidget#maincontextphoto"
            "{"
            "image: url(:/Themes/Assets/Themes/backgrounds/beach/2560x1600.jpg);"

            "}"

            );

        ui->maincontextphoto_2->setStyleSheet("QWidget#maincontextphoto_2"
                                              "{"
                                              "image: url(:/Themes/Assets/Themes/backgrounds/beach/2560x1600.jpg);"

                                              "}");
        ui->tomato_clock_background->setStyleSheet("QWidget#tomato_clock_background"
                                                   "{"
                                                       "image: url(:/Themes/Assets/Themes/backgrounds/beach/2560x1600.jpg);"

                                                  " }");
        ui->deepseek_background->setStyleSheet("QWidget#deepseek_background"
                                               "{"
                                                   "image: url(:/Themes/Assets/Themes/backgrounds/beach/2560x1600.jpg);"

                                               "}");
    }

    emit change_photo_clicked(id);
}


void Widget::on_MydayButton_clicked()
{

}


void Widget::on_settings_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void Widget::on_dateEdit_userDateChanged(const QDate &date)
{
    int dayofwww = date.dayOfWeek();
    semesterStart = date.addDays(1 - dayofwww);
    QDate currentDate = QDate::currentDate();

    // 获取当前日期是周几（Qt: Monday=1, Sunday=7）
    int dayOfWeek = currentDate.dayOfWeek();

    // 计算本周的周一和周日
    QDate monday = currentDate.addDays(1 - dayOfWeek);
    QDate sunday = currentDate.addDays(7 - dayOfWeek);

    // 格式化为 MM.dd - MM.dd（换行）
    QString weekRange = QString("%1 - %2\n")
                            .arg(monday.toString("MM.dd"))
                            .arg(sunday.toString("MM.dd"));

    // 计算第几周
    int days = semesterStart.daysTo(monday);
    selected_weekNumber = days / 7 + 1;

    // 拼接完整文本
    QString labelText = weekRange + QString("   第 %1 周").arg(selected_weekNumber);

    // 设置到界面 label
    ui->nowdate_week->setText(labelText);
}


void Widget::on_back_to_home_week_clicked()
{
    QDate currentDate = QDate::currentDate();

    // 获取当前日期是周几（Qt: Monday=1, Sunday=7）
    int dayOfWeek = currentDate.dayOfWeek();

    // 计算本周的周一和周日
    QDate monday = currentDate.addDays(1 - dayOfWeek);
    QDate sunday = currentDate.addDays(7 - dayOfWeek);

    // 格式化为 MM.dd - MM.dd（换行）
    QString weekRange = QString("%1 - %2\n")
                            .arg(monday.toString("MM.dd"))
                            .arg(sunday.toString("MM.dd"));

    // 计算第几周
    int days = semesterStart.daysTo(monday);
    selected_weekNumber = days / 7 + 1;

    // 拼接完整文本
    QString labelText = weekRange + QString("   第 %1 周").arg(selected_weekNumber);

    // 设置到界面 label
    ui->nowdate_week->setText(labelText);
    ui->back_to_home_week->setVisible(false);

    draw_class_buttons_from_vector();
}


void Widget::on_lineEdit_2_returnPressed()
{
    QString city = ui->lineEdit_2->text();
    ui->lineEdit_2->setEnabled(false);

    QString weatherinfo = WeatherFetcher().getWeatherString(city);



    if(weatherinfo != "error")
    {
        QString weathertext;
        QString weathericon;
        for(int i = 0; i < 2; i++)
        {
            weathericon += weatherinfo[i];
        }

        QString temperature;

        int weatherIndex = weatherinfo.indexOf("天气：");
        int tempIndex = weatherinfo.indexOf("温度：");

        if (weatherIndex != -1 && tempIndex != -1 && tempIndex > weatherIndex) {
            weathertext = weatherinfo.mid(weatherIndex + 3, tempIndex - (weatherIndex + 3)).trimmed();
            temperature = weatherinfo.mid(tempIndex + 3).trimmed();
        }

        // 设置 UI
        ui->weather_icon->setText(weathericon);  // 这里你没说怎么提取图标，如果有图标符号可以单独提取
        ui->WeatherLabel->setText(weathertext + "\n" + temperature);

        ui->city_name->setText(city);
        ui->lineEdit_2->setEnabled(true);
    }
    else {
        ui->WeatherLabel->setText("获取失败");
        ui->lineEdit_2->setEnabled(true);
    }
}


void Widget::on_TaskButton_clicked()
{

}





void Widget::on_study_time_set_valueChanged(int arg1)
{
    work_time = arg1;
}




void Widget::on_rest_time_set_valueChanged(int arg1)
{
    rest_time = arg1;
}


void Widget::updateTime()
{
    realtime_real = realtime_real.addSecs(1);
    ui->label_20->setText(realtime_real.toString("mm:ss"));

    if(current_mode == 0)
        ui->now_progres_line->setProgress((realtime_real.minute() * 1.0 * 60 + realtime_real.second()) / (work_time * 60));
    else
        ui->now_progres_line->setProgress((realtime_real.minute() * 1.0 * 60 + realtime_real.second()) / (rest_time * 60));

    if(current_mode == 0 && realtime_real.minute() == work_time)
    {

        // tomatoWidget->setState(TomatoClock::TomatoState::Rest, TomatoClock::RunState::Running);

            current_mode = 1;
            ui->now_progres_line->setProgress(0);
            ui->now_progres_line->colorwho = 1;
            ui->pushButton->setStyleSheet("border: none;"
"background-color: rgba(5, 111, 5,50);"
"color: rgb(83, 188, 45);");
            ui->pushButton->setText("休息时间");
            realtime_real.setHMS(0, 0, 0);
            ui->label_20->setText(realtime_real.toString("mm:ss"));
            fanqie_num++;
            ui->label_8->setText("第" + QString::number(fanqie_num) + "个番茄钟");
    }
    else if(current_mode == 1 && realtime_real.minute() == rest_time)
    {
        current_mode = 0;
        ui->now_progres_line->setProgress(0);
        ui->now_progres_line->colorwho = 0;
        ui->pushButton->setStyleSheet(
            "border: none;"
            "background-color: rgba(79, 2, 20,50);"
            "color: rgb(227, 79, 99);"
            );
        ui->pushButton->setText("工作时间");
        realtime_real.setHMS(0, 0, 0);
        ui->label_20->setText(realtime_real.toString("mm:ss"));
    }
}

void Widget::on_tomato_clock_start_button_clicked()
{
    realtime.start();
    ui->tomato_clock_start_button->setVisible(false);
    ui->tomato_clock_stop_button->setVisible(true);
    ui->tomato_clock_reset_button->setVisible(true);
    ui->small_tomato_clock_button->setVisible(true);

    ui->study_time_set->setEnabled(false);
    ui->rest_time_set->setEnabled(false);
}


void Widget::on_tomato_clock_stop_button_clicked()
{
    if(realtime.isActive())
    {
        realtime.stop();
        if(tomatoWidget != nullptr) tomatoWidget->pause();
        ui->tomato_clock_stop_button->setText(QString("继续"));

    }
    else {
        realtime.start();
        if(tomatoWidget != nullptr) tomatoWidget->start();
        ui->tomato_clock_stop_button->setText(QString("暂停"));
    }
}


void Widget::on_small_tomato_clock_button_clicked()
{
    if(tomatoWidget == nullptr)
    {
        tomatoWidget = new TomatoClock(nullptr, &realtime);
        tomatoWidget ->setAttribute(Qt::WA_DeleteOnClose); // 自动内存管理
        connect(tomatoWidget, &TomatoClock::windowClosed,this,&Widget::handleTomatoClockClosed);
        connect(tomatoWidget, &TomatoClock::tomatoClockstop, this, [this]()
                {
                    if(realtime.isActive())
                    {
                        ui->tomato_clock_stop_button->setText(QString("暂停"));
                    }
                    else {

                        ui->tomato_clock_stop_button->setText(QString("继续"));
                    }
                });
        tomatoWidget->setWorkDuration(work_time);
        tomatoWidget->setRestDuration(rest_time);
        tomatoWidget->updateExternalTime(realtime_real.second() + realtime_real.minute() * 60);

        connect(tomatoWidget, &TomatoClock::roundFinished, this, [](TomatoClock::TomatoState state){
            if (state == TomatoClock::Work)
                qDebug() << "完成了一个番茄工作！进入休息。";
            else
                qDebug() << "休息结束！进入工作。";
        });
        tomatoWidget->setWindowFlag(Qt::WindowStaysOnTopHint);  // 设置置顶窗口

        // 获取主屏幕几何尺寸
        QScreen *screen = QGuiApplication::screenAt(QCursor::pos());
        QRect screenGeometry = screen->availableGeometry();

        // 计算显示位置（右下角，稍微上移避免贴边）
        int x = screenGeometry.right() - tomatoWidget->width() - 20;
        int y = screenGeometry.bottom() - tomatoWidget->height() - 20;

        tomatoWidget->move(x, y);  // 移动到右下角

        tomatoWidget->show();
    }
}


void Widget::on_tomato_clock_reset_button_clicked()
{
    realtime.stop();
    realtime_real.setHMS(0, 0, 0);
    ui->label_20->setText(realtime_real.toString("mm:ss"));
    ui->tomato_clock_start_button->setVisible(true);
    ui->tomato_clock_stop_button->setVisible(false);
    ui->tomato_clock_reset_button->setVisible(false);
    ui->small_tomato_clock_button->setVisible(false);
    current_mode = 0;
    ui->now_progres_line->setProgress(0);
    ui->now_progres_line->colorwho = 0;
    if(tomatoWidget != nullptr)
    {
        tomatoWidget->resetTime();
        tomatoWidget->pause();
        tomatoWidget->close();
        handleTomatoClockClosed();
    }
    ui->pushButton->setStyleSheet(
        "border: none;"
        "background-color: rgba(79, 2, 20,50);"
        "color: rgb(227, 79, 99);"
        );
    ui->pushButton->setText("工作时间");
    ui->study_time_set->setEnabled(true);
    ui->rest_time_set->setEnabled(true);
}





void Widget::on_pushButton_15_clicked()
{
    // loadClassesFromFile();
    // loadTodosFromFile();
    ui->pushButton_15->setEnabled(false);
    update_deepseek_suggestion();

}


void Widget::on_clear_temp_cache_clicked()
{
    // created_class_buttons->clear();
    // created_small_buttons->clear();
    // created_large_buttons->clear();
}


void Widget::on_TaskButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void  Widget::update_deepseek_suggestion()
{
    // struct a_class
    // {
    //     QVector<bool>weekinfo;
    //     QString classname;
    //     QString classinfo;
    //     QString classid;
    //     QColor thecolor;
    //     int which_weekday = 1;
    //     bool use_precise_time = false;
    //     QTime begintime;
    //     QTime endtime;
    //     int beginclass = 1;
    //     int endclass = 1;
    // };
    // 获取当前日期
    QDate currentDate = QDate::currentDate();

    // 获取当前日期是周几（Qt: Monday=1, Sunday=7）
    int dayOfWeek = currentDate.dayOfWeek();

    // 计算本周的周一和周日
    QDate monday = currentDate.addDays(1 - dayOfWeek);
    QDate sunday = currentDate.addDays(7 - dayOfWeek);

    // 格式化为 MM.dd - MM.dd（换行）
    QString weekRange = QString("%1 - %2\n")
                            .arg(monday.toString("MM.dd"))
                            .arg(sunday.toString("MM.dd"));

    // 计算第几周
    int days = semesterStart.daysTo(monday);
    int wselected_weekNumber = days / 7 + 1;

    QVector<a_class> today_class;
    for(int i = 0; i < created_class_buttons->size(); i++)
    {
        if((*created_class_buttons)[i]->weekinfo[wselected_weekNumber - 1] && (*created_class_buttons)[i]->which_weekday == dayOfWeek)
        {
            a_class temp;
            temp.classname = (*created_class_buttons)[i]->classname;
            temp.classinfo = (*created_class_buttons)[i]->classinfo;
            temp.beginclass = (*created_class_buttons)[i]->beginclass;
            temp.endclass = (*created_class_buttons)[i]->endclass;
            temp.use_precise_time = (*created_class_buttons)[i]->use_precise_time;
            temp.begintime = (*created_class_buttons)[i]->begintime;
            temp.endtime = (*created_class_buttons)[i]->endtime;
            today_class.push_back(temp);
        }
    }

    // struct a_todo
    // {
    //     QString title;
    //     QString info;
    //     QString id;
    //     QDate deadline;
    //     QTime deadtime;
    //     QString cycleline;
    //     bool is_have_deadline = false;
    //     bool is_have_deadtime = false;
    //     bool is_have_cycle = false;
    //     bool is_have_info = false;
    // };

    QVector<a_todo> today_todo;
    for(int i = 0; i < created_large_buttons->size(); i++)
    {
        qDebug() << (*created_large_buttons)[i]->title;
            a_todo temp;
            temp.title = (*created_large_buttons)[i]->title;
            temp.info = (*created_large_buttons)[i]->info;
            temp.id = (*created_large_buttons)[i]->id;
            temp.deadline = (*created_large_buttons)[i]->deadline;
            temp.deadtime = (*created_large_buttons)[i]->deadtime;
            temp.cycleline = (*created_large_buttons)[i]->cycleline;
            today_todo.push_back(temp);

    }

    QString prompt = "我是一名北京大学的学生，过着中国大学里的生活节奏，有早上八点的课的时候我一般7点40起，除此之外我一般9：30起床，晚上我大概会在12点之后睡觉，我上课的地方平均离宿舍5分钟路程，一节课时间是50分钟，第一节课8:00 - 8: 50 , 第二节课9: 00 - 9 : 50, 第三节课10:10 - 11:00 , 第四节课11:10 - 12:00 , 第五节课13:00 - 13：50 ， 第六节课14：00 - 14：50， 第七节课15：10 - 16：00 ， 第八节课16：10 - 17：00，第九节课18：40-19：20，第十节课19：30-20：20。请根据以下课程和待办事项，为我制定一份合理的今日日程安排，合理安排各项任务的时间。你不需要在对话结束后继续追问，我只会询问你一次\n\n";

    // 添加课程信息
    prompt += "今日课程安排如下：\n";

    for (const a_class &cls : today_class) {
        QString timeStr;
        if (cls.use_precise_time) {
            timeStr = QString("%1 - %2").arg(cls.begintime.toString("HH:mm")).arg(cls.endtime.toString("HH:mm"));
        } else {
            timeStr = QString("第 %1 节 - 第 %2 节").arg(cls.beginclass).arg(cls.endclass);
        }
        prompt += QString("- 课程：%1，时间：%2\n").arg(cls.classname, timeStr);
    }
    if(today_class.size() == 0)
    {
        prompt += QString("今天没有课程安排。\n");
    }

    // 添加待办信息
    prompt += "\n今日待办事项如下：\n";
    for (const a_todo &todo : today_todo) {
        QString todoStr = QString("- 标题：%1").arg(todo.title);

        if (todo.is_have_info && !todo.info.isEmpty())
            todoStr += QString("，详情：%1").arg(todo.info);

        if (todo.is_have_deadline)
            todoStr += QString("，截止日期：%1").arg(todo.deadline.toString("yyyy-MM-dd"));

        if (todo.is_have_deadtime)
            todoStr += QString(" %1").arg(todo.deadtime.toString("HH:mm"));

        if (todo.is_have_cycle)
            todoStr += QString("，周期：%1").arg(todo.cycleline);

        prompt += todoStr + "\n";
    }


    if(today_todo.size() == 0)
    {
        prompt += QString("今天没有待办事项。\n");
    }


    // 加入请求说明
    prompt += "\n请在考虑课程时间的前提下，合理安排今日待办事项的完成时间，并避免时间冲突。如果有空余时间，也可以安排休息或复盘时间。";

    // 调用模型请求
    temp.setApiKey(QString("sk-e453fe48eee14f0598370477d903bf76"));
    qDebug()<<prompt;
    temp.sendPrompt(prompt);

    connect(&temp, &DeepSeekClient::responseReady, [this](QString reply)
            {
        qDebug() << "Response: " << reply;
        ui->everyday_segestion_box->document()->setMarkdown(reply);
        ui->pushButton_15->setEnabled(true);
    }
            );

    connect(&temp, &DeepSeekClient::errorOccurred, [this](QString error)
            {
        qDebug() << "Error: " << error;
    }
            );
}





void Widget::on_input_question_returnPressed()
{
    chat.setApiKey("sk-e453fe48eee14f0598370477d903bf76");
    QString question_text = ui->input_question->text().trimmed();
    if (question_text.isEmpty()) return;

    ui->input_question->clear();

    // 显示用户消息
    appendMarkdownToChat(QString("**你：** %1").arg(question_text));

    // 添加到历史
    chatHistory.append(qMakePair(QString("user"), question_text));

    // 构造完整 prompt（模拟 messages 多轮结构）
    QString system_intro = "请基于上下文回答用户问题。";
    QString prompt;
    prompt += system_intro + "\n";

    for (const auto &msg : chatHistory) {
        prompt += QString("[%1]: %2\n").arg(msg.first, msg.second);
    }

    // 连接响应信号（单次连接）
    connect(&chat, &DeepSeekClient::responseReady, this, [=](const QString &response) mutable {
        appendMarkdownToChat(QString("**DS：** %1").arg(response));
        chatHistory.append(qMakePair(QString("assistant"), response));
        ui->input_question->setEnabled(true);
    });

    connect(&chat, &DeepSeekClient::errorOccurred, this, [=](const QString &error) {
        ui->chat_result->append(QString("<p style='color:#d32f2f; font-size:14px'><b>错误：</b> %1</p>").arg(error.toHtmlEscaped()));
        ui->input_question->setEnabled(true);
    });

    // 发出请求
    chat.sendPrompt(prompt);
    ui->input_question->setEnabled(false);
}


void Widget::appendMarkdownToChat(const QString &markdownText)
{
    QTextCursor cursor = ui->chat_result->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertBlock();

    QTextDocumentFragment fragment = QTextDocumentFragment::fromMarkdown(markdownText);
    cursor.insertFragment(fragment);
}


