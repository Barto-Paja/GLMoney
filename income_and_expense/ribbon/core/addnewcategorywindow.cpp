#include "addnewcategorywindow.hpp"
#include "ui_addnewcategorywindow.h"

#include <QMessageBox>

AddNewCategoryWindow::AddNewCategoryWindow(Data *data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddNewCategoryWindow),
    m_data(data)
{
    ui->setupUi(this);
}

AddNewCategoryWindow::~AddNewCategoryWindow()
{
    delete ui;
}

void AddNewCategoryWindow::on_pushButton_Commit_clicked()
{
    QString error_msg;
    if(ui->lineEdit_name->text().isEmpty())
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setText("Pole z nazwą nie może być puste!");
        msg.addButton(QMessageBox::Ok);
        msg.exec();
    }
    else
    {
        if(m_data->addCategory(ui->lineEdit_name->text(),error_msg))
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Information);
            msg.setText("Transakcja dodana pomyślnie!");
            msg.addButton(QMessageBox::Ok);
            msg.exec();

            this->close();
        }
        else
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Critical);
            msg.setText("Dodanie kontrahenta nie powiodło się!\n"+error_msg);
            msg.addButton(QMessageBox::Ok);
            msg.exec();
        }
    }
}

void AddNewCategoryWindow::on_pushButton_canel_clicked()
{
    close();
}
