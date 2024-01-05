#include "mainmenu.hpp"
#include "ui_mainmenu.h"
#include <QMessageBox>
#include <QDebug>

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenu)
{
    qDebug() << "SHA: " << GIT_HASH;
    ui->setupUi(this);
    QString header{"Budżet domowy\nWersja: 1.0.0."+QString{GIT_HASH}};
    ui->label_title->setText(header);
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

void MainMenu::on_pushButton_About_clicked()
{
    QMessageBox msg {QMessageBox::Icon::Information,
                "O aplikacji",
                "Informacje o aktualizacjach, usterki, sugestie i nowe funkcjonalności można zgłaszać w projekcie pdo adresem "
                "https://gitlab.com/Barto-Paja/GLMoney",
                QMessageBox::StandardButton::Ok
               };

    msg.exec();
}

