#include "use_webengine.h"
#ifdef USE_WEB
#include <QJsonObject>
#include"browserwindow.h"
#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QDir>
#include <QStringList>
#include <QRegularExpression>
#include <QRandomGenerator>
#include <QTextStream>
#include <QColor>

#ifndef SAVEDATA_H
#define SAVEDATA_H


struct CourseInfo {
    QString name;          // 课程名称
    int start_week;        // 起始周
    int end_week;          // 终止周
    int which_day;
    int which_time;
    QString week_type;     // 周期类型
    QString location;      // 上课地点
    QString teacher;       // 教师
    QString notes;         // 备注
};

class Savedata{
public:
    bool weekinfo[20];
    QString classname;//名字
    QString classinfo;//备注(包含地点\老师\其他信息)
    QColor thecolor;// 按钮颜色
    int which_weekday =1;//default : Mon
    bool use_precise_time = false;//采用哪种方式计时
    QTime begintime;
    QTime endtime;
    int begin_class =1;// 从第几节课开始
    int end_class =1;//第几节课结束
public:
    Savedata();
    Savedata(CourseInfo info);
    void print();
    void save_to_file(QTextStream& O);
    void read_from_file(QTextStream& I);
    QJsonObject toJson()const;
    void fromJson(const QJsonObject& json);

};

QString extractSubstring(const QString& str, const QString& s1, const QString& s2);
QColor generateRandomBackgroundColor();
int draw_data_from_html();
int save_to_json(QString outfilepath, QVector<Savedata>& classes);

QVector<Savedata> get_data_from_web();
QVector<Savedata> read_from_txt(QString filename);
QVector<Savedata> read_from_json(QString filename);

QString createDirectoryInAppPath(const QString &dirName);

#endif // SAVEDATA_H

#endif
