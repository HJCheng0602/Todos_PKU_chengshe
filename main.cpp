#include "widget.h"
#include "mainwindow.h"
#include "datecalendar.h"
#include "subrepeat.h"
#include "smallwidget.h"
#include <QIcon>


#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QVector<CustomButton *> * created_large_buttons = new QVector<CustomButton *>;
    QVector<SmallCustomButton*> * created_small_buttons = new QVector<SmallCustomButton *>;

    QApplication a(argc, argv);
    Widget w;
    smallwidget e;
    w.created_large_buttons = created_large_buttons;
    w.created_small_buttons = created_small_buttons;
    w.setWindowTitle(QString("Todos"));
    w.setWindowIcon(QIcon(":/Themes/Assets/Square44x44Logo.targetsize-256.png"));

    e.setWindowTitle(QString("Small Todos"));
    e.setWindowIcon(QIcon(":/Themes/Assets/Square44x44Logo.targetsize-256.png"));

    w.show();


    e.created_small_buttons = created_small_buttons;
    e.created_large_buttons = created_large_buttons;
    e.hide(); // 如果需要初始显示 smallwidget，否则调用 e.hide()
    w.po = &e;
    e.po = &w;
    w.loadTodosFromFile();
    w.update_deepseek_suggestion();
    e.change_background(w.background_ID);
    // 连接 Widget 的按钮点击信号到槽函数
    QObject::connect(&w, &Widget::changesizeButtonClicked, &e, [&w, &e]() {
        w.hide();
        e.show();
        e.draw_todo_buttons_from_vector();
    });



    // 连接 smallwidget 的按钮点击信号到槽函数
    QObject::connect(&e, &smallwidget::changesizeButtonClicked, &w, [&w, &e]() {
        e.hide();
        w.show();
        w.draw_todo_buttons_from_vector();
    });

    QObject::connect(&a, &QApplication::aboutToQuit, [&w]() {
        w.saveTodosToFile();  // 程序退出时保存
    });

    QObject::connect(&w, &Widget::change_photo_clicked, [&e](int id) {
        e.change_background(id);
    });

    return a.exec();
}
