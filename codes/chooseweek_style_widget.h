#ifndef CHOOSEWEEK_STYLE_WIDGET_H
#define CHOOSEWEEK_STYLE_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVector>

namespace Ui {
class chooseweek_style_widget;
}

class chooseweek_style_widget : public QWidget
{
    Q_OBJECT

public:
    explicit chooseweek_style_widget(QWidget *parent = nullptr);
    QVector<bool>m_selectedWeeks;

    void clear_redraw();
    ~chooseweek_style_widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_51_clicked();

    void on_saveButton_clicked();

private:
    Ui::chooseweek_style_widget *ui;
    void setButtonStyle(QPushButton* btn, bool isSelected);

signals:
    void data_saved(QVector<bool> i);

};

#endif // CHOOSEWEEK_STYLE_WIDGET_H
