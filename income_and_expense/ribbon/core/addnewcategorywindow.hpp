#ifndef ADDNEWCATEGORYWINDOW_HPP
#define ADDNEWCATEGORYWINDOW_HPP

#include <QWidget>
#include "data.hpp"

namespace Ui {
class AddNewCategoryWindow;
}

class AddNewCategoryWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AddNewCategoryWindow(Data *data, QWidget *parent = nullptr);
    ~AddNewCategoryWindow();

private slots:
    void on_pushButton_Commit_clicked();
    void on_pushButton_canel_clicked();

private:
    Ui::AddNewCategoryWindow *ui;

    Data * m_data { nullptr };
};

#endif // ADDNEWCATEGORYWINDOW_HPP
