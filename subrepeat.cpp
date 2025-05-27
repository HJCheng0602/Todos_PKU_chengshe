#include "subrepeat.h"
#include "ui_subrepeat.h"
#include<QRegularExpressionValidator>

subrepeat::subrepeat(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::subrepeat)
{
    ui->setupUi(this);
    ui->comboBox->addItem("天");
    ui->comboBox->addItem("周");
    ui->comboBox->addItem("月");
    ui->comboBox->addItem("年");
    ui->lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]+$")));

    this->setWindowTitle(QString("循环设置"));
    this->setWindowIcon(QIcon(":/Themes/Assets/repeat.svg"));




}

subrepeat::~subrepeat()
{
    delete ui;
}

void subrepeat::on_comboBox_currentIndexChanged(int index)
{
    circle_unit = index;
}


void subrepeat::on_lineEdit_textChanged(const QString &arg1)
{
    int i = arg1.toInt();
    unit_num = i;
}


void subrepeat::on_ConcellButton_clicked()
{
    close();
}


void subrepeat::on_SaveButton_clicked()
{
    QPair<int, int>pair(unit_num, circle_unit);
    emit datas_save(pair);
    close();
}

