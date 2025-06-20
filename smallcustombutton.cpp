#include "SmallCustomButton.h"
#include "widget.h"

SmallCustomButton::SmallCustomButton(const QString &text, const QString &id, QWidget *parent) : QWidget(parent) {
    // 主按钮
    mainButton = new QPushButton(this);
    mainButton->setObjectName(id);
    mainButton->setText("");
    this->id = id;
    mainButton->setMinimumSize(QSize(30, 40));
    mainButton->setStyleSheet("QPushButton { "
                              "background-color: rgba(0, 0, 0, 180); /* 黑色半透明 */ "
                              "color: white; /* 文字颜色为白色 */ "
                              "border: none; /* 去掉边框 */ "
                              "border-bottom-left-radius: 10px;"
                              "border-top-left-radius: 10px;"
                              "icon: url(:/Themes/Assets/circle-large.svg); /* 正常状态图标 */ "
                              "padding-left: 10px;"
                              "text-align: left; /* 文字居左 */ "
                              "}"
                              "QPushButton:hover { "
                              "background-color: rgba(0, 0, 0, 128); /* 悬浮时背景颜色变深 */ "
                              "icon: url(:/Themes/Assets/circle-check.svg); /* 正常状态图标 */ "
                              "}");

    // 叠加的修改按钮
    editButton = new QPushButton("修改", this);
    title = text;
    editButton->setText(title);
    editButton->setMinimumSize(QSize(187, 40));
    editButton->setStyleSheet("QPushButton { "
                              "background-color: rgba(0, 0, 0, 180); "
                              "color: white; "
                              "border: none; "
                              "text-align: left; /* 文字居左 */ "
                              "}"
                              "QPushButton:hover { "
                              "background-color: rgba(0, 0, 0, 128); /* 悬浮时背景颜色变深 */ "
                              "}"
                              );
    deleteButton = new QPushButton("dele", this);
    deleteButton->setText("");
    deleteButton->setMinimumSize(QSize(30, 40));
    deleteButton->setStyleSheet("QPushButton { "
                                "background-color: rgba(0, 0, 0, 180); /* 黑色半透明 */ "
                                "color: white; /* 文字颜色为白色 */ "
                                "border: none; /* 去掉边框 */ "
                                "border-bottom-right-radius: 10px;"
                                "border-top-right-radius: 10px;"
                                "icon: url(:/Themes/Assets/delete.svg); /* 正常状态图标 */ "
                                "padding-left: 10px;"
                                "text-align: left; /* 文字居左 */ "
                                "}"
                                "QPushButton:hover { "
                                "background-color: rgba(0, 0, 0, 128); /* 悬浮时背景颜色变深 */ "
                                "icon: url(:/Themes/Assets/delete.svg); /* 正常状态图标 */ "
                                "}");


    // 布局设置
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(mainButton);
    layout->setAlignment(mainButton, Qt::AlignLeft);
    layout->addWidget(editButton);
    layout->setAlignment(editButton, Qt::AlignCenter);
    layout->addWidget(deleteButton);
    layout->setAlignment(deleteButton, Qt::AlignRight);

    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    // 连接信号与槽
    connect(editButton, &QPushButton::clicked, this, &SmallCustomButton::onEditButtonClicked);
    connect(mainButton, &QPushButton::clicked, this, &SmallCustomButton::onMainButtonClicked);
    connect(deleteButton, &QPushButton::clicked, this, &SmallCustomButton::onDeleteButtonClicked);
}

void SmallCustomButton::onEditButtonClicked() {
    emit editButtonClicked(id);
    qDebug()<<"now";
}

void SmallCustomButton::onMainButtonClicked()
{
    emit mainButtonClicked(id);
}

void SmallCustomButton::onDeleteButtonClicked()
{
    emit deleteButttonClicked(id);
}



void SmallCustomButton::redraw()
{

    QString finaltext = title;
    finaltext = finaltext + QString('\n');
    if(is_have_deadline)
    {
        finaltext += deadline.toString("MM-dd") + QString(" ");
    }

    if(is_have_deadtime)
    {
        finaltext += deadtime.toString("截止到hh:00") + QString(" ");
    }

    if(is_have_cycle)
    {
        finaltext += cycleline + QString(" ");
    }

    if(is_have_info)
    {
        finaltext += QString("\U0001F3F7");
    }
    editButton->setText(finaltext);
    if(isfinished)
    {
        mainButton->setStyleSheet("QPushButton { "
                                  "background-color: rgba(0, 0, 0, 180); /* 黑色半透明 */ "
                                  "color: white; /* 文字颜色为白色 */ "
                                  "border: none; /* 去掉边框 */ "
                                  "border-bottom-left-radius: 10px;"
                                  "border-top-left-radius: 10px;"
                                  "icon: url(:/Themes/Assets/finish.svg); /* 正常状态图标 */ "
                                  "padding-left: 10px;"
                                  "text-align: left; /* 文字居左 */ "
                                  "}"
                                  "QPushButton:hover { "
                                  "background-color: rgba(0, 0, 0, 180); /* 悬浮时背景颜色变深 */ "
                                  "icon: url(:/Themes/Assets/finish.svg); /* 正常状态图标 */ "
                                  "}");
        mainButton->setEnabled(false);
        editButton->setEnabled(false);
    }
}




