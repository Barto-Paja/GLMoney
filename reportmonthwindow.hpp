#ifndef REPORTMONTHWINDOW_HPP
#define REPORTMONTHWINDOW_HPP

#include <QWidget>

#include <QChart>

#include "data.hpp"

namespace Ui {
class ReportMonthWindow;
}

class ReportMonthWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ReportMonthWindow(Data *data, QWidget *parent = nullptr);
    ~ReportMonthWindow();

private:
    Ui::ReportMonthWindow *ui;

    Data * m_data { nullptr };
};

#endif // REPORTMONTHWINDOW_HPP
