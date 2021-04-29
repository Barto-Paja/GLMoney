#include "newmemberwindow.hpp"
#include "ui_newmemberwindow.h"

#include <QMessageBox>

NewMemberWindow::NewMemberWindow(Data *data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewMemberWindow),
    m_data(data)
{
    ui->setupUi(this);
}

NewMemberWindow::~NewMemberWindow()
{
    delete ui;
}

void NewMemberWindow::on_pushButton_Commit_clicked()
{
    QString error_msg;

    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setText("Pole z nazwą nie może być puste!");
        msg.addButton(QMessageBox::Ok);
        msg.exec();
    }
    else
    {
        if(m_data->addMember(ui->lineEdit->text(),error_msg))
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Information);
            msg.setText(ui->lineEdit->text()+" dodane pomyślnie!");
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

void NewMemberWindow::on_pushButton_Canel_clicked()
{
    close();
}
