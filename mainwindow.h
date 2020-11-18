#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "data.hpp"
#include "newpayeewindow.hpp"
#include "reportmonthwindow.hpp"

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
    void on_comboBox_category_currentIndexChanged(int index);

    void on_pushButton_4_clicked();

    void on_calendarWidget_clicked(const QDate &date);

    void on_checkBox_isIncome_stateChanged(int arg1);

    void on_pushButton_Commit_clicked();

    void on_pushButton_Back_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_actionDodaj_Nowego_atnika_triggered();

    void on_actionRaport_roczny_triggered();

private:
    Ui::MainWindow *ui;

    Data m_data;

    void fillComboboxes();
    void reloadSubcategories(int index);
    void reloadPayees();
};
#endif // MAINWINDOW_H
