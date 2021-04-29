#include "newaccountwindow.hpp"
#include "ui_newaccountwindow.h"

#include <QMessageBox>


NewAccountWindow::NewAccountWindow(Data *data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewAccountWindow),
    m_data(data)
{
    ui->setupUi(this);
}

NewAccountWindow::~NewAccountWindow()
{
    delete ui;
}

void NewAccountWindow::on_pushButton_Commit_clicked()
{
    QString error_msg;
    if(ui->lineEdit_Name->text().isEmpty())
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setText("Pole z nazwą nie może być puste!");
        msg.addButton(QMessageBox::Ok);
        msg.exec();
    }
    else
    {
        if(m_data->addAccount(ui->lineEdit_Name->text(),
                               ui->lineEdit_Description->text(),
                               error_msg))
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Information);
            msg.setText(ui->lineEdit_Name->text()+" dodano pomyślnie!");
            msg.addButton(QMessageBox::Ok);
            msg.exec();

            this->close();
        }
        else
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Critical);
            msg.setText("Dodanie konta nie powiodło się!\n"+error_msg);
            msg.addButton(QMessageBox::Ok);
            msg.exec();
        }
    }
}

void NewAccountWindow::on_pushButton_Canel_clicked()
{
    close();
}
