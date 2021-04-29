#include "mainmenu.hpp"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::on_pushButton_noteIncomesAndExpenses_clicked()
{
    emit menuItem(1);
}

void MainMenu::on_pushButton_budgetPlane_clicked()
{
    emit menuItem(2);
}

void MainMenu::on_pushButton_Quit_clicked()
{
    emit quit();
}
