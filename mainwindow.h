#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "data.hpp"
#include "homebudget/homebudget_form.hpp"
#include "income_and_expense/incomesandexpenses_form.hpp"
#include "mainmenu.hpp"
#include "income_and_expense/ribbon/report/payeereportwindow.hpp"
#include "income_and_expense/ribbon/report/expensereportwindow.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_payeereport_clicked();

    void on_pushButton_categoryreport_clicked();

private:
    Ui::MainWindow *ui;
    Data m_data;

    PayeeReportWindow *m_payeeReportWindow {nullptr};
    ExpenseReportWindow *m_expenseReportWindow {nullptr};
};
#endif // MAINWINDOW_H
