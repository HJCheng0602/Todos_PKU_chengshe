#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QString>
#include <QPair>
#include <QDate>
#include <QTime>

struct a_todo
{
    QString title;
    QString info;
    QString id;
    QDate deadline;
    QTime deadtime;
    QString cycleline;
    bool is_have_deadline = false;
    bool is_have_deadtime = false;
    bool is_have_cycle = false;
    bool is_have_info = false;
};

class CustomButton : public QWidget {
    Q_OBJECT

public:
    explicit CustomButton(const QString &text, const QString &id, QWidget *parent = nullptr);
    QString id;
    QString title;
    QString info;

    QString cycleline;
    QDate deadline;
    QTime deadtime;

    bool is_have_deadline = false;
    bool is_have_deadtime = false;
    bool is_have_cycle = false;
    bool is_have_info = false;
    bool isfinished = false;
    void redraw();
    QPair<int, int>pair = QPair<int, int>(0, -1);
    QPushButton *mainButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
signals:
    void editButtonClicked(QString id);
    void mainButtonClicked(QString id);
    void deleteButttonClicked(QString id);

private slots:
    void onEditButtonClicked();
    void onMainButtonClicked();
    void onDeleteButtonClicked();

private:

};

#endif // CUSTOMBUTTON_H
