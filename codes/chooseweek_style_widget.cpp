#include "chooseweek_style_widget.h"
#include "ui_chooseweek_style_widget.h"

chooseweek_style_widget::chooseweek_style_widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::chooseweek_style_widget)
{
    ui->setupUi(this);
    // 初始化选中状态数组，默认所有周未被选中
    for (int i = 0; i < 20; ++i) {
        m_selectedWeeks.push_back(true);
    }

    // 为每个按钮设置点击事件处理函数
    for (int i = 1; i <= 20; ++i) {
        QPushButton* btn = this->findChild<QPushButton*>(QString("weekpushButton_%1").arg(i));
        if (btn) {
            // 设置初始样式
            setButtonStyle(btn, true);
            // 连接按钮的点击信号到槽函数
            connect(btn, &QPushButton::clicked, this, [this, btn, i]() {
                // 获取按钮对应的周数索引（从0开始）
                int weekIndex = i - 1;
                // 切换选中状态
                m_selectedWeeks[weekIndex] = !m_selectedWeeks[weekIndex];
                // 更新按钮样式
                setButtonStyle(btn, m_selectedWeeks[weekIndex]);
            });
        }
    }
}


void chooseweek_style_widget::clear_redraw()
{
    for(int i = 0; i < 20; i++)
    {
        m_selectedWeeks[i] = true;
        QPushButton* btn = this->findChild<QPushButton*>(QString("weekpushButton_%1").arg(i + 1));
        if (btn)
        {
            setButtonStyle(btn, true);
        }
    }
}



chooseweek_style_widget::~chooseweek_style_widget()
{
    delete ui;
}

// 辅助函数：根据选中状态设置按钮样式
void chooseweek_style_widget::setButtonStyle(QPushButton* btn, bool isSelected)
{
    if (isSelected) {
        btn->setStyleSheet("QPushButton {"
                           "border-radius: 15px;"
                           "background-color: rgba(100, 180, 250, 200);"

                           "}"
                           "QPushButton:hover {"
                           "background-color: rgba(120, 190, 255, 200);"

                           "}");
    } else {
        btn->setStyleSheet("QPushButton {"
                           "border-radius: 15px;"
                           "}"
                           "QPushButton:hover {"
                           "background-color: rgba(200, 200, 200, 200);"
                           "}");
    }
}

void chooseweek_style_widget::on_pushButton_clicked()
{
    for (int i = 1; i <= 20; ++i) {
        QPushButton* btn = this->findChild<QPushButton*>(QString("weekpushButton_%1").arg(i));
        if (btn) {
            m_selectedWeeks[i - 1] = true;
            setButtonStyle(btn, true);
        }
    }

}


void chooseweek_style_widget::on_pushButton_2_clicked()
{
    for (int i = 1; i <= 20; ++i) {
        QPushButton* btn = this->findChild<QPushButton*>(QString("weekpushButton_%1").arg(i));
        if (btn) {
            if(i % 2 == 1)
            {
                m_selectedWeeks[i - 1] = true;
                setButtonStyle(btn, true);
            }
            else {
                m_selectedWeeks[i - 1] = false;
                setButtonStyle(btn, false);
            }
        }
    }
}


void chooseweek_style_widget::on_pushButton_3_clicked()
{
    for (int i = 1; i <= 20; ++i) {
        QPushButton* btn = this->findChild<QPushButton*>(QString("weekpushButton_%1").arg(i));
        if (btn) {
            if(i % 2 == 0)
            {
                m_selectedWeeks[i - 1] = true;
                setButtonStyle(btn, true);
            }
            else {
                m_selectedWeeks[i - 1] = false;
                setButtonStyle(btn, false);
            }
        }
    }
}


void chooseweek_style_widget::on_pushButton_51_clicked()
{
    close();
}


void chooseweek_style_widget::on_saveButton_clicked()
{
    emit data_saved(m_selectedWeeks);
    close();
}

