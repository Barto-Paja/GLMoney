#ifndef NEWPAYEEWINDOW_HPP
#define NEWPAYEEWINDOW_HPP

#include <QWidget>
#include "data.hpp"

namespace Ui {
class NewPayeeWindow;
}

class NewPayeeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit NewPayeeWindow(Data * data, QWidget *parent = nullptr);
    ~NewPayeeWindow();

private slots:
    void on_pushButton_Commit_clicked();

private:
    Ui::NewPayeeWindow *ui;

    Data * m_data { nullptr };
};

#endif // NEWPAYEEWINDOW_HPP
