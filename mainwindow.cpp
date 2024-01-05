#include "mainwindow.h"
#include "ui_mainwindow.h"

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

}

MainWindow::~MainWindow()
{
    delete ui;
}
