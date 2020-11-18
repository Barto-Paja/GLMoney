#include "newpayeewindow.hpp"
#include "ui_newpayeewindow.h"

#include <QMessageBox>

NewPayeeWindow::NewPayeeWindow(Data *data, QWidget *parent) :
      QWidget(parent),
    ui(new Ui::NewPayeeWindow),
    m_data(data)
{
    ui->setupUi(this);
}

NewPayeeWindow::~NewPayeeWindow()
{
    delete ui;
}

void NewPayeeWindow::on_pushButton_Commit_clicked()
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
        if(m_data->addPayee(ui->lineEdit_name->text(),ui->lineEdit_description->text(),error_msg))
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
