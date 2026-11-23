#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "income_and_expense/ribbon/report/payeereportwindow.hpp"
#include "income_and_expense/ribbon/report/expensereportwindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMainWindow::setWindowState(Qt::WindowMaximized);

    ui->widget_homebudget = new HomeBudget_Form(&m_data,ui->widget_homebudget);
    connect(qobject_cast<HomeBudget_Form*>(ui->widget_homebudget),&HomeBudget_Form::back,this,[&](){ui->stackedWidget->setCurrentIndex(0);});

    ui->widget_mainMenu = new MainMenu(ui->widget_mainMenu);
    connect(qobject_cast<MainMenu*>(ui->widget_mainMenu),&MainMenu::menuItem,this,[&](int index){ui->stackedWidget->setCurrentIndex(index);});
    connect(qobject_cast<MainMenu*>(ui->widget_mainMenu),&MainMenu::quit,this,[&](){exit(0);});

    ui->widget_notesIncomeAndExpense = new IncomesAndExpenses_Form(&m_data,ui->widget_notesIncomeAndExpense);
    connect(qobject_cast<IncomesAndExpenses_Form*>(ui->widget_notesIncomeAndExpense),&IncomesAndExpenses_Form::back,this,[&](){ui->stackedWidget->setCurrentIndex(0);});

    // Raport kontrahenta
    m_payeeReportWindow = new PayeeReportWindow(&m_data, this);

    // Raport wydatków
    m_expenseReportWindow = new ExpenseReportWindow(&m_data, this);

    // Dodanie raportów do QStackedWidget
    ui->stackedWidget->addWidget(m_payeeReportWindow);
    ui->stackedWidget->addWidget(m_expenseReportWindow);

    connect(m_payeeReportWindow,
            &PayeeReportWindow::backToAnalytics,
            this,
            [this]() {
                ui->stackedWidget->setCurrentIndex(3);
            });

    connect(m_expenseReportWindow,
            &ExpenseReportWindow::backToAnalytics,
            this,
            [this]() {
                ui->stackedWidget->setCurrentIndex(3);
            });

    connect(ui->pushButton_back,
            &QPushButton::clicked,
            this,
            [this]() {
                ui->stackedWidget->setCurrentIndex(0);
            });

    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_payeereport_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_pushButton_categoryreport_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

