#ifndef NEWSUBCATEGORYWINDOW_HPP
#define NEWSUBCATEGORYWINDOW_HPP

#include <QWidget>
#include "data.hpp"

namespace Ui {
class NewSubcategoryWindow;
}

class NewSubcategoryWindow : public QWidget
{
    Q_OBJECT

public:
    explicit NewSubcategoryWindow(Data * data, QWidget *parent = nullptr);
    ~NewSubcategoryWindow();

private slots:
    void on_pushButton_Commit_clicked();

    void on_pushButton_Canel_clicked();

private:
    Ui::NewSubcategoryWindow *ui;
    Data * m_data { nullptr };
};

#endif // NEWSUBCATEGORYWINDOW_HPP
