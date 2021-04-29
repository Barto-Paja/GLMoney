#ifndef NEWACCOUNTWINDOW_HPP
#define NEWACCOUNTWINDOW_HPP

#include <QWidget>
#include "data.hpp"

namespace Ui {
class NewAccountWindow;
}

class NewAccountWindow : public QWidget
{
    Q_OBJECT

public:
    explicit NewAccountWindow(Data * data, QWidget *parent = nullptr);
    ~NewAccountWindow();

private slots:
    void on_pushButton_Commit_clicked();

    void on_pushButton_Canel_clicked();

private:
    Ui::NewAccountWindow *ui;
    Data * m_data { nullptr };
};

#endif // NEWACCOUNTWINDOW_HPP
