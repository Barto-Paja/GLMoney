#ifndef NEWMEMBERWINDOW_HPP
#define NEWMEMBERWINDOW_HPP

#include <QWidget>
#include "data.hpp"

namespace Ui {
class NewMemberWindow;
}

class NewMemberWindow : public QWidget
{
    Q_OBJECT

public:
    explicit NewMemberWindow(Data * data, QWidget *parent = nullptr);
    ~NewMemberWindow();

private slots:
    void on_pushButton_Commit_clicked();

    void on_pushButton_Canel_clicked();

private:
    Ui::NewMemberWindow *ui;
    Data * m_data { nullptr };
};

#endif // NEWMEMBERWINDOW_HPP
