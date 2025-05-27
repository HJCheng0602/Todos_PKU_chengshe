#ifndef WIDGET_H
#define WIDGET_H

#include "use_webengine.h"

#include <QWidget>
#include <QMainWindow>
#include <QVariantMap>
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QThread>
#include <QFile>
#include <QVBoxLayout>
#include <QPushButton>
#include <QApplication>
#include<QDate>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QDir>
#include "datecalendar.h"
#include "subrepeat.h"
#include "QVector"
#include "custombutton.h"
#include "smallcustombutton.h"
#include "filehandler.h"
#include "editclasswidget.h"
#include "edit_class_button.h"
#ifdef USE_WEB
#include "savedata.h"
#endif
#include <QStandardPaths>
//#include "class_custom_button.h"
#include "tomatoclock.h"
#include "deepseekclient.h"
#include <QPair>



QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class smallwidget;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    QVector<CustomButton*> *created_large_buttons = new QVector<CustomButton*>;
    QVector<SmallCustomButton *> *created_small_buttons = new QVector<SmallCustomButton*>;
    QVector<edit_class_button *> * created_class_buttons = new QVector<edit_class_button*>;
    void draw_todo_buttons_from_vector();
    void draw_class_buttons_from_vector();
    smallwidget * po;
    ~Widget();

    void loadTodosFromFile();
    void saveTodosToFile();
    void loadClassesFromFile();
    void saveClassesToFile();
    void change_background(int id);
    void updateTime();
    void update_deepseek_suggestion();
    static QString getDataPath(QString filename);
    QString DATA_PATH;
    int print_model = 0; //0 : 暗色 , 1 : 亮色

    QDate semesterStart = QDate(2025, 2, 17);


    QTimer realtime;
    QTime realtime_real = QTime(0, 0, 0);

    int work_time = 25;
    int rest_time = 5;

    int current_mode = 0; //0 : 工作 , 1 : 休息

    int fanqie_num = 1;
    DeepSeekClient temp;
    DeepSeekClient chat;
    QString apiKey;

    QVector<QPair<QString, QString>> chatHistory;

    void appendMarkdownToChat(const QString &markdownText);
    int background_ID;
private:
    Ui::Widget *ui;
    datecalendar w;
    datecalendar v;
    subrepeat c;
    editclasswidget e;
    QDate todo_set_date = QDate::currentDate();

    int selected_weekNumber = -1;

    QString line; //放到搜索框右边的信息
    QString repeat_info = "无循环";
    bool is_todo_date_changed = false;
    QTime todo_set_time = QTime::currentTime();
    bool is_todo_time_changed = false;
    int now_edit_bbutton = -1;

    int now_edit_class = -1;
    int now_week = 0;
    int unitnum = 0;
    int unittype = -1;

    TomatoClock *tomatoWidget = nullptr;

    // const QString DATA_PATH = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
    //                           + "/todos.json";

public slots:
    void open_edit_menu(const QString &id);
    void mark_finished(const QString &id);
    void mark_deleted(const QString &id);
    void open_class_edit_menu(const QString &id);
    void edit_class(int i);


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



    void on_add_class_from_web_clicked();

    void on_add_class_by_hand_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_MydayButton_clicked();

    void on_settings_button_clicked();

    void on_dateEdit_userDateChanged(const QDate &date);

    void on_back_to_home_week_clicked();

    void on_lineEdit_2_returnPressed();

    void showTomatoClock();
    void handleTomatoClockClosed();

    void on_TaskButton_clicked();



    void on_study_time_set_valueChanged(int arg1);

    void on_rest_time_set_valueChanged(int arg1);



    void on_tomato_clock_start_button_clicked();

    void on_tomato_clock_stop_button_clicked();

    void on_small_tomato_clock_button_clicked();

    void on_tomato_clock_reset_button_clicked();


    void on_pushButton_15_clicked();

    void on_clear_temp_cache_clicked();

    void on_TaskButton_2_clicked();

    void on_input_question_returnPressed();



signals:
    void changesizeButtonClicked();
    void change_photo_clicked(int id);
};
#endif // WIDGET_H
