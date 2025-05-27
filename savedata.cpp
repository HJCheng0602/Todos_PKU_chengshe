#include "use_webengine.h"
#ifdef USE_WEB

#include "header.h"
#include "widget.h"
QString Week[8] = {"", "mon", "tue", "wed", "thu", "fri", "sat", "sun"};
QTime class_start_time[13]={QTime(0,0),
                              QTime(8,0),QTime(9,0),QTime(10,10),QTime(11,10),
                              QTime(13,0),QTime(14,0),QTime(15,10),QTime(16,10),
                              QTime(17,10),QTime(18,40),QTime(19,40),QTime(20,40)};

QTime class_end_time[13]={QTime(0,0),
                            QTime(8,50),QTime(9,50),QTime(11,00),QTime(12,00),
                            QTime(13,50),QTime(14,50),QTime(16,00),QTime(17,00),
                            QTime(18,00),QTime(19,30),QTime(20,30),QTime(21,30)};

Savedata::Savedata(){}

Savedata::Savedata(CourseInfo info){
    classname=info.name;
    classinfo=info.notes;
    which_weekday=info.which_day;
    begin_class=info.which_time;
    end_class=info.which_time;
    begintime=class_start_time[begin_class];
    endtime=class_end_time[begin_class];
    thecolor=generateRandomBackgroundColor();
    bool flag[2]={true,true};
    if(info.week_type=="单周"){
        flag[0]=false;
    }
    else if(info.week_type=="双周"){
        flag[1]=false;
    }
    memset(weekinfo,0,sizeof(bool[20]));
    for(int i=info.start_week;i<=info.end_week;i++){
        if(flag[i%2]){
            weekinfo[i]=true;
        }
    }
}

void Savedata::print(){
    qDebug()<<"";
    qDebug()<<"课程名称: "<<classname;
    QString s="在以下周上课：";
    for(int i=0;i<20;i++){
        if(weekinfo[i])
            s=s+" "+QString::number(i);
    }
    qDebug().noquote()<<s;
    qDebug()<<"按钮颜色: "<<thecolor;
    qDebug()<<"星期"<<which_weekday;
    if(use_precise_time) qDebug()<<"采用精确计时：是";
    else qDebug()<<"采用精确计时：否";
    qDebug()<<"上课时间(QTime格式)："<<begintime.toString("hh:mm");
    qDebug()<<"下课时间(QTime格式)："<<endtime.toString("hh:mm");
    qDebug()<<"开始节数："<<begin_class;
    qDebug()<<"结束节数："<<end_class;
}

void Savedata::save_to_file(QTextStream& O){
    O<<classname<<Qt::endl;
    for(int i=0;i<20;i++){
        if(weekinfo[i]){
            O<<"1 ";
        }
        else{
            O<<"0 ";
        }
    }
    O<<Qt::endl<<thecolor.name(QColor::HexArgb)<<Qt::endl;
    O<<which_weekday<<Qt::endl;
    if(use_precise_time) O<<1<<Qt::endl;
    else O<<0<<Qt::endl;
    O<<begintime.toString("hh mm")<<" "<<endtime.toString("hh mm")<<Qt::endl;
    O<<begin_class<<" "<<end_class<<Qt::endl;
    return;
}

void Savedata::read_from_file(QTextStream& I){
    I>>classname;
    int tmp;
    for(int i=0;i<20;i++){
        I>>tmp;
        weekinfo[i]=(tmp==1);
    }
    QString colorstr;
    I>>colorstr;
    thecolor=QColor(colorstr);
    I>>which_weekday;
    I>>tmp;
    use_precise_time=(tmp==1);
    int hh,mm;
    I>>hh>>mm;
    begintime=QTime(hh,mm);
    I>>hh>>mm;
    endtime=QTime(hh,mm);
    I>>begin_class>>end_class;
    return;
}
QJsonObject Savedata::toJson()const{
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
    obj["begin_class"]=begin_class;
    obj["end_class"]=end_class;
    obj["classinfo"]=classinfo;
    return obj;
}

void Savedata::fromJson(const QJsonObject& json){
    classname=json["classname"].toString();
    QJsonArray weekinfos=json["weekinfo"].toArray();
    int i=0;
    for(const QJsonValue& val:weekinfos){
        weekinfo[i]=val.toBool();
        i++;
    }
    classinfo=json["classinfo"].toString();
    thecolor=QColor(json["thecolor"].toString());
    which_weekday=json["which_weekday"].toInt();
    use_precise_time=json["use_precise_time"].toBool();
    begintime=QTime::fromString(json["begintime"].toString().trimmed(),"hh:mm");
    endtime=QTime::fromString(json["endtime"].toString().trimmed(),"hh:mm");
    begin_class=json["begin_class"].toInt();
    end_class=json["end_class"].toInt();
    return;
}


QString extractSubstring(const QString& str, const QString& s1, const QString& s2){
    // 1. 查找 s1 的起始位置
    int pos_s1 = str.indexOf(s1);
    if (pos_s1 == -1) {
        qWarning() << "s1 not found in the string.";
        return QString();
    }

    // 2. 计算起始位置（s1 结束后）
    int start = pos_s1 + s1.length();

    // 3. 查找 s2 的起始位置（从 start 开始）
    int pos_s2 = str.indexOf(s2, start);
    if (pos_s2 == -1) {
        qWarning() << "s2 not found after s1.";
        return QString();
    }

    // 4. 提取子串（不包含 s1 和 s2）
    return str.mid(start, pos_s2 - start);
}
QColor generateRandomBackgroundColor(){
    QRandomGenerator* rand = QRandomGenerator::global();

    // 随机色相（0-360度）
    double hue = rand->bounded(360.0);

    // 较高饱和度（50%-100%）
    double saturation = rand->bounded(51) / 100.0 + 0.5; // [0.5, 1.01) -> 实际最大 1.0

    // 控制明度在中等范围（40%-60%）
    double lightness = rand->bounded(21) / 100.0 + 0.4;  // [0.4, 0.61) -> 实际最大 0.6

    // 创建 HSL 颜色
    QColor color;
    color.setHslF(hue / 360.0, saturation, lightness);

    return color;
}
int draw_data_from_html(){
    // qDebug() << "当前工作目录:" << QDir::currentPath();
    // QString filename = "data2.html";
    QString filename=Widget::getDataPath("saved_page.html");
    QString target;
    QVector<Savedata> classes;
    // 读取文件内容
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "无法打开文件";
        return 1;
    }
    QString content = QString::fromUtf8(file.readAll());
    file.close();

    for (int j = 1; j <= 7; ++j) {
        for (int i = 1; i <= 12; ++i) {
            target = Week[j] + QString::number(i);
            QString s1="<span style=\"font-size:12px;\">";
            QString s2="</span></div></td>";
            int pos = content.indexOf(target);
            if (pos == -1) {
                qWarning() << "未找到子串:" << target;
                continue;
            }

            // 截取目标后的内容
            QString remaining = extractSubstring(content, target, s2);

            // 清理 HTML 标签
            int spanPos = remaining.indexOf(s1);
            if (spanPos != -1) {
                remaining = remaining.mid(spanPos + s1.length());
            }

            if(remaining.length()==0) continue;

            // 分割行
            QStringList lines = remaining.split("<br>", Qt::SkipEmptyParts);

            // 解析课程信息
            CourseInfo info;
            if (!lines.isEmpty()) {
                info.name = lines[0];
                info.which_day=j;
                info.which_time=i;
                if (lines.size() >= 2) {
                    QString class_info = lines[1];
                    QRegularExpression re(R"((\d+)-(\d+)周\s+(\S+周)\s+(\S+)\s+教师：(\S+))");
                    QRegularExpressionMatch match = re.match(class_info);

                    if (match.hasMatch()) {
                        info.start_week = match.captured(1).toInt();
                        info.end_week = match.captured(2).toInt();
                        info.week_type = match.captured(3);
                        info.location = match.captured(4);
                        info.teacher = match.captured(5);
                    }

                    int notePos = class_info.indexOf("备注：");
                    if (notePos != -1) {
                        info.notes = class_info.mid(notePos + 3);
                    }
                }
            }

            //输出日志
            // qDebug().noquote() << "\n" << target << ":";
            // qDebug() << "课程名称:" << info.name;
            // qDebug() << "上课时间:" << info.which_day<<" "<<info.which_time;
            // qDebug() << "周数范围:" << QString("%1-%2周").arg(info.start_week).arg(info.end_week);
            // qDebug() << "周期类型:" << info.week_type;
            // qDebug() << "上课地点:" << info.location;
            // qDebug() << "教师姓名:" << info.teacher;
            // qDebug() << "备注信息:" << info.notes << "\n";

            Savedata finaldata(info);
            if(classes.isEmpty()){
                classes.push_back(finaldata);
            }
            else{
                if(classes.last().classname==finaldata.classname
                    && classes.last().which_weekday==finaldata.which_weekday
                    && classes.last().end_class==finaldata.begin_class-1){
                    classes.last().end_class++;
                    classes.last().endtime=class_end_time[classes.last().end_class];
                }
                else{
                    classes.push_back(finaldata);
                }
            }
        }
    }
    // for(auto i=classes.begin();i!=classes.end();++i){
    //     i->print();
    // }
    save_to_json(Widget::getDataPath("saved_data.json"),classes);
    return 0;
}


int save_to_json(QString outfilepath, QVector<Savedata>& classes){
    QJsonArray jsonArray;
    for(const Savedata& obj:classes){
        jsonArray.append(obj.toJson());
    }
    QJsonDocument doc(jsonArray);
    QFile file(outfilepath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "error" << file.errorString();
        return -1;
    }
    file.write(doc.toJson(QJsonDocument::Indented));
    return 0;
}


QVector<Savedata> read_from_txt(QString filename){
    QFile file(filename);
    // 以只读模式打开文件，如果文件不存在则创建
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件:" << file.errorString();
        return {};
    }
    QTextStream I(&file);
    int class_cnt;
    I>>class_cnt;
    QVector<Savedata> classes(class_cnt);
    for(auto i=classes.begin();i!=classes.end();++i){
        i->read_from_file(I);
    }
    return classes;
}


QVector<Savedata> read_from_json(QString filename){
    QVector<Savedata> result;
    QFile file(filename);
    // 以只读模式打开文件，如果文件不存在则创建
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件:" << file.errorString();
        return result;
    }
    QByteArray data=file.readAll();
    QJsonDocument doc=QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isArray()) {
        qWarning() << "JSON 格式错误或根不是数组！";
        return result;
    }
    QJsonArray jsonArray = doc.array();
    for (const QJsonValue& value : jsonArray) {
        if (value.isObject()) {
            Savedata obj;
            obj.fromJson(value.toObject());
            result.push_back(obj);
        } else {
            qWarning() << "发现非对象元素，已跳过";
        }
    }

    return result;
}

QVector<Savedata> get_data_from_web(){
    BrowserWindow::openBrowserAndSaveHtml(QUrl("https://portal.pku.edu.cn"));
    draw_data_from_html();
    return read_from_json(Widget::getDataPath("saved_data.json"));
}

QString createDirectoryInAppPath(const QString &dirName) {
    // 获取可执行文件所在目录
    QString appDir = QCoreApplication::applicationDirPath();

    // 拼接目标路径
    QDir targetDir(appDir);
    if (!targetDir.exists(dirName)) {
        if (targetDir.mkdir(dirName)) {
            qDebug() << "Directory created:" << targetDir.filePath(dirName);
            return targetDir.filePath(dirName);
        } else {
            qDebug() << "Failed to create directory:" << targetDir.filePath(dirName);
            return "";
        }
    } else {
        qDebug() << "Directory already exists:" << targetDir.filePath(dirName);
        return targetDir.filePath(dirName);
    }
}

#endif
