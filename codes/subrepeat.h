#ifndef SUBREPEAT_H
#define SUBREPEAT_H

#include <QWidget>

namespace Ui {
class subrepeat;
}

class subrepeat : public QWidget
{
    Q_OBJECT

public:
    explicit subrepeat(QWidget *parent = nullptr);
    ~subrepeat();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_ConcellButton_clicked();

    void on_SaveButton_clicked();

private:
    Ui::subrepeat *ui;
    int circle_unit = -1;
    QString text;
    int unit_num = 0;
signals:
    void datas_save(QPair<int, int> & selected);
};

#endif // SUBREPEAT_H
