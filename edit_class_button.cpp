#include "edit_class_button.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
edit_class_button::edit_class_button(QString id,QWidget *parent)
    : QPushButton{parent}
{
    classid = id;
    for(int i = 0; i < 20; i++)
    {
        weekinfo.push_back(true);
    }
    //mainButton = new QPushButton(this);

    int button_x = 0;
    int button_y = 0;
    int button_height = 0;
    if(this->which_weekday == 1)
    {
        button_x = 81;
    }
    else if(this->which_weekday == 2)
    {
        button_x = 163;
    }
    else if(this->which_weekday == 3)
    {
        button_x = 245;
    }
    else if(this->which_weekday == 4)
    {
        button_x = 327;
    }
    else if(this->which_weekday == 5)
    {
        button_x = 409;
    }
    else if(this->which_weekday == 6)
    {
        button_x = 491;
    }
    else if(this->which_weekday == 7)
    {
        button_x = 571;
    }
    if(this->use_precise_time) // 使用精确时间
    {
        int miniutenum = this->begintime.hour() * 60 + this->begintime.minute();
        button_y = 36 + ((miniutenum - 480) / 800) * 451;
        int miniutenum2 = this->endtime.hour() * 60 + this->endtime.minute();
        int button_y2 = 36 + ((miniutenum2 - 480) / 800) * 451;
        button_height = button_y2 - button_y;
    }
    else {
        if(this->beginclass == 1) {
            button_y = 36;
        }
        else if(this->beginclass == 2)
        {
            button_y = 77;
        }
        else if(this->beginclass == 3)
        {
            button_y = 118;
        }
        else if(this->beginclass == 4)
        {
            button_y = 159;
        }
        else if(this->beginclass == 5)
        {
            button_y = 200;
        }
        else if(this->beginclass == 6)
        {
            button_y = 241;
        }
        else if(this->beginclass == 7)
        {
            button_y = 282;
        }
        else if(this->beginclass == 8)
        {
            button_y = 323;
        }
        else if(this->beginclass == 9)
        {
            button_y = 364;
        }
        else if(this->beginclass ==10)
        {
            button_y = 405;
        }
        else if(this->beginclass ==11)
        {
            button_y = 446;
        }

        if(this->endclass == 1)
        {
            button_height = 77 - button_y;
        }
        else if(this->endclass == 2)
        {
            button_height = 118 - button_y;
        }
        else if(this->endclass == 3)
        {
            button_height = 159 - button_y;
        }
        else if(this->endclass == 4)
        {
            button_height = 200 - button_y;
        }
        else if(this->endclass == 5)
        {
            button_height = 241 - button_y;
        }
        else if(this->endclass == 6)
        {
            button_height = 282 - button_y;
        }
        else if(this->endclass == 7)
        {
            button_height = 323 - button_y;
        }
        else if(this->endclass == 8)
        {
            button_height = 364 - button_y;
        }
        else if(this->endclass ==9)
        {
            button_height = 405 - button_y;
        }
        else if(this->endclass ==10)
        {
            button_height = 446 - button_y;
        }
        else if(this->endclass == 11)
        {
            button_height = 487 - button_y;
        }
    }

    //this->setParent(parentwidget);
    this->resize(70, button_height);
    this->move(button_x, button_y);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(20);               // 模糊程度
    effect->setOffset(0, 0);                 // 阴影偏移
    effect->setColor(QColor(255, 255, 255)); // 发光颜色（蓝紫光）

    this->setGraphicsEffect(effect);
    // 设置样式和文本
    QColor baseColor = this->thecolor.isValid() ? this->thecolor : QColor("blue");
    baseColor = baseColor.lighter(70);
    QColor hoverColor = baseColor.lighter(120);
    QString styleSheet = QString(
                             "QPushButton {\n"
                             "max-width: 70px;"
                             "    background-color: %1;\n"
                             "    color: white;\n"
                             "    border: 8px;\n"
                             "    border-color: white;\n"
                             "    border-radius: 5px;\n"
                             "    padding: 10px;\n"
                             "    font-size: 8px;\n"
                             "}\n"

                             "QPushButton:hover {\n"
                             "    background-color: %2;\n"
                             "}\n"
                             ).arg(baseColor.name()).arg(hoverColor.name());


    this->setStyleSheet(styleSheet);
    //this->setText(this->classname);
    // 假设 this 是 QPushButton 派生类内部
    text.setText(this->classname);        // 设置文本
    text.setParent(this);                 // 设置为按钮子控件
    text.setFixedWidth(50);               // 限定宽度以触发换行
    text.setWordWrap(true);               // 启用自动换行

    // 居中显示：设置大小并计算居中位置
    int label_width = text.width();       // 50
    int label_height = text.sizeHint().height();  // 根据文字自动估计高度
    int button_width = this->width();
    int bbutton_height = this->height();

    // 水平和垂直居中移动 label
    text.move((button_width - label_width) / 2, (bbutton_height - label_height) / 2);

    // 设置样式：透明背景 + 文字颜色
    text.setStyleSheet("background: transparent; color: white;");

    // 对齐方式：文字内容在 label 内部居中
    text.setAlignment(Qt::AlignCenter);
    //this->setWordWrap(true);

    connect(this, &QPushButton::clicked, this, &edit_class_button::onMainButtonClicked);

}

void edit_class_button::print(){
    // QString classid;
    qDebug()<<classid;
    // QString classname;
    qDebug()<<classname;
    // QString classinfo;
    qDebug()<<classinfo;
    // QTime begintime;
    // QTime endtime;
    qDebug()<<begintime.toString("hh mm")<<" "<<endtime.toString("hh mm");
    // bool use_precise_time = false;
    if(use_precise_time) qDebug()<<1;
    else qDebug()<<0;
    // int beginclass = 1;
    // int endclass = 1;
    qDebug()<<beginclass<<" "<<endclass;
    // QColor thecolor = QColor(255, 255, 255);
    qDebug()<<thecolor.name(QColor::HexArgb);
    // int which_weekday = 1;
    qDebug()<<which_weekday;
    // QVector<bool>weekinfo;
    for(auto it=weekinfo.begin();it!=weekinfo.end();++it){
        if(*it){qDebug().noquote()<<"1 ";}
        else{qDebug().noquote()<<"0 ";}
    }
    return;
}

void edit_class_button::saveToFile(QTextStream& O){
    // QString classid;
    O<<classid<<Qt::endl;
    // QString classname;
    O<<classname<<Qt::endl;
    // QString classinfo;
    O<<classinfo<<Qt::endl;
    // QTime begintime;
    // QTime endtime;
    O<<begintime.toString("hh mm")<<" "<<endtime.toString("hh mm")<<Qt::endl;
    // bool use_precise_time = false;
    if(use_precise_time) O<<1<<Qt::endl;
    else O<<0<<Qt::endl;
    // int beginclass = 1;
    // int endclass = 1;
    O<<beginclass<<" "<<endclass<<Qt::endl;
    // QColor thecolor = QColor(255, 255, 255);
    O<<thecolor.name(QColor::HexArgb)<<Qt::endl;
    // int which_weekday = 1;
    O<<which_weekday<<Qt::endl;
    // QVector<bool>weekinfo;
    for(auto it=weekinfo.begin();it!=weekinfo.end();++it){
        if(*it){O<<"1 ";}
        else{O<<"0 ";}
    }
    O<<Qt::endl;
    return;
}


void edit_class_button::readFromFile(QTextStream& I){
    // QString classid;
    I>>classid;
    // QString classname;
    I>>classname;
    // QString classinfo;
    I>>classinfo;
    // QTime begintime;
    // QTime endtime;
    int hh,mm;
    I>>hh>>mm;
    begintime=QTime(hh,mm);
    I>>hh>>mm;
    endtime=QTime(hh,mm);
    // bool use_precise_time = false;
    int tmp;
    I>>tmp;
    use_precise_time=(tmp==1);
    // int beginclass = 1;
    // int endclass = 1;
    I>>beginclass>>endclass;
    // QColor thecolor = QColor(255, 255, 255);
    QString colorstr;
    I>>colorstr;
    thecolor=QColor(colorstr);
    // int which_weekday = 1;
    I>>which_weekday;
    // QVector<bool>weekinfo;
    weekinfo=QVector<bool>(20);
    for(int i=0;i<20;i++){
        I>>tmp;
        weekinfo[i]=(tmp==1);
    }
    return;
}


QJsonObject edit_class_button::toJson() const{
    QJsonObject obj;
    obj["classname"]=classname;
    QJsonArray weekinfos;
    for(int i=0;i<20;i++){
        weekinfos.append(weekinfo[i]);
    }
    obj["weekinfo"]=weekinfos;
    obj["thecolor"]=thecolor.name(QColor::HexArgb);
    obj["which_weekday"]=which_weekday;
    obj["use_precise_time"]=use_precise_time;
    obj["begintime"]=begintime.toString("hh:mm");
    obj["endtime"]=endtime.toString("hh:mm");
    obj["begin_class"]=beginclass;
    obj["end_class"]=endclass;
    obj["classinfo"]=classinfo;
    obj["classid"]=classid;
    qDebug() << "when save" << classid;
    return obj;
}
void edit_class_button::fromJson(const QJsonObject& json){
    classname=json["classname"].toString();
    QJsonArray weekinfos=json["weekinfo"].toArray();
    weekinfo.clear();
    for(const QJsonValue& val:weekinfos){
        weekinfo.push_back(val.toBool());
    }
    classinfo=json["classinfo"].toString();
    classid=json["classid"].toString();
    thecolor=QColor(json["thecolor"].toString());
    which_weekday=json["which_weekday"].toInt();
    use_precise_time=json["use_precise_time"].toBool();
    begintime=QTime::fromString(json["begintime"].toString().trimmed(),"hh:mm");
    endtime=QTime::fromString(json["endtime"].toString().trimmed(),"hh:mm");
    beginclass=json["begin_class"].toInt();
    endclass=json["end_class"].toInt();
    return;
}


void edit_class_button::redraw()
{
    int button_x = 0;
    int button_y = 0;
    int button_height = 0;
    if(this->which_weekday == 1)
    {
        button_x = 81;
    }
    else if(this->which_weekday == 2)
    {
        button_x = 163;
    }
    else if(this->which_weekday == 3)
    {
        button_x = 245;
    }
    else if(this->which_weekday == 4)
    {
        button_x = 327;
    }
    else if(this->which_weekday == 5)
    {
        button_x = 409;
    }
    else if(this->which_weekday == 6)
    {
        button_x = 491;
    }
    else if(this->which_weekday == 7)
    {
        button_x = 571;
    }
    if(this->use_precise_time) // 使用精确时间
    {
        int miniutenum = this->begintime.hour() * 60 + this->begintime.minute();
        button_y = 36 + ((miniutenum - 480) / 800) * 451;
        int miniutenum2 = this->endtime.hour() * 60 + this->endtime.minute();
        int button_y2 = 36 + ((miniutenum2 - 480) / 800) * 451;
        button_height = button_y2 - button_y;
    }
    else {
        if(this->beginclass == 1) {
            button_y = 36;
        }
        else if(this->beginclass == 2)
        {
            button_y = 77;
        }
        else if(this->beginclass == 3)
        {
            button_y = 118;
        }
        else if(this->beginclass == 4)
        {
            button_y = 159;
        }
        else if(this->beginclass == 5)
        {
            button_y = 200;
        }
        else if(this->beginclass == 6)
        {
            button_y = 241;
        }
        else if(this->beginclass == 7)
        {
            button_y = 282;
        }
        else if(this->beginclass == 8)
        {
            button_y = 323;
        }
        else if(this->beginclass == 9)
        {
            button_y = 364;
        }
        else if(this->beginclass ==10)
        {
            button_y = 405;
        }
        else if(this->beginclass ==11)
        {
            button_y = 446;
        }

        if(this->endclass == 1)
        {
            button_height = 77 - button_y;
        }
        else if(this->endclass == 2)
        {
            button_height = 118 - button_y;
        }
        else if(this->endclass == 3)
        {
            button_height = 159 - button_y;
        }
        else if(this->endclass == 4)
        {
            button_height = 200 - button_y;
        }
        else if(this->endclass == 5)
        {
            button_height = 241 - button_y;
        }
        else if(this->endclass == 6)
        {
            button_height = 282 - button_y;
        }
        else if(this->endclass == 7)
        {
            button_height = 323 - button_y;
        }
        else if(this->endclass == 8)
        {
            button_height = 364 - button_y;
        }
        else if(this->endclass ==9)
        {
            button_height = 405 - button_y;
        }
        else if(this->endclass ==10)
        {
            button_height = 446 - button_y;
        }
        else if(this->endclass == 11)
        {
            button_height = 487 - button_y;
        }
    }



    //this->setParent(parentwidget);

    this->resize(70, button_height);
    this->move(button_x, button_y);


    // 设置样式和文本
    QColor baseColor = this->thecolor.isValid() ? this->thecolor : QColor("blue");
    baseColor = baseColor.lighter(70);
    QColor hoverColor = baseColor.lighter(120);
    QString styleSheet = QString(
                             "QPushButton {\n"
                             "max-width: 70px;"
                             "    background-color: %1;\n"
                             "    color: white;\n"
                             "    border: 8px;\n"
                             "    border-color: white;\n"
                             "    border-radius: 5px;\n"
                             "    padding: 10px;\n"
                             "    font-size: 8px;\n"
                             "}\n"

                             "QPushButton:hover {\n"
                             "    background-color: %2;\n"
                             "}\n"
                             ).arg(baseColor.name()).arg(hoverColor.name());
    // mainButton.setStyleSheet(styleSheet);
    // mainButton.setText(this->classname + QString("09"));
    // mainButton.setVisible(is_visi);
    // mainButton.setEnabled(true);

    this->setStyleSheet(styleSheet);
    //this->setText(this->classname);
    // 假设 this 是 QPushButton 派生类内部
    text.setText(this->classname);        // 设置文本
    text.setParent(this);                 // 设置为按钮子控件
    text.setFixedWidth(50);               // 限定宽度以触发换行
    text.setWordWrap(true);               // 启用自动换行

    // 居中显示：设置大小并计算居中位置
    int label_width = text.width();       // 50
    int label_height = text.sizeHint().height();  // 根据文字自动估计高度
    int button_width = this->width();
    int bbutton_height = this->height();

    // 水平和垂直居中移动 label
    text.move((button_width - label_width) / 2, (bbutton_height - label_height) / 2);

    // 设置样式：透明背景 + 文字颜色
    text.setStyleSheet("background: transparent; color: white;");

    // 对齐方式：文字内容在 label 内部居中
    text.setAlignment(Qt::AlignCenter);
    this->setVisible(is_visi);
    this->setEnabled(true);

    // qDebug()<< "here!";
    // qDebug() << this->classname;
    // qDebug() << button_height;
    // qDebug() << button_y;
    // qDebug() << button_x;
    //this->show();


}


void edit_class_button::onMainButtonClicked()
{
    qDebug() << "Edit button clicked!" << this->classname << this->classid;
    emit mainbuttonclicked(classid);
}
