#include "payeeinformationwindow.hpp"
#include "ui_payeeinformationwindow.h"

PayeeInformationWindow::PayeeInformationWindow(Data *data, QWidget *parent) :
    QDialog(parent),
    ui{new Ui::PayeeInformationWindow},
    m_data{data}
{
    ui->setupUi(this);
}

PayeeInformationWindow::~PayeeInformationWindow()
{
    delete ui;
}
