#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "data.hpp"
#include "homebudget/homebudget_form.hpp"
#include "income_and_expense/incomesandexpenses_form.hpp"
#include "mainmenu.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Data m_data;
};
#endif // MAINWINDOW_H
