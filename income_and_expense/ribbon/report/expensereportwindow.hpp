#ifndef EXPENSEREPORTWINDOW_HPP
#define EXPENSEREPORTWINDOW_HPP

#include <QWidget>
#include "data.hpp"

namespace Ui {
class ExpenseReportWindow;
}

class ExpenseReportWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ExpenseReportWindow(Data *data, QWidget *parent = nullptr);
    ~ExpenseReportWindow();

signals:
    void backToAnalytics();

private slots:
    void generateReport();

    void on_pushButton_back_clicked();

private:
    Ui::ExpenseReportWindow *ui;
    Data *m_data;

    void setupTable();
    void updateTotal(double total);
};

#endif // EXPENSEREPORTWINDOW_HPP
