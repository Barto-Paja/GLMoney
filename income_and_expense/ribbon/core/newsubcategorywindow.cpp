#include "newsubcategorywindow.hpp"
#include "ui_newsubcategorywindow.h"

#include <QMessageBox>

NewSubcategoryWindow::NewSubcategoryWindow(Data *data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewSubcategoryWindow),
    m_data(data)
{
    ui->setupUi(this);

    for(auto i : m_data->categories())
    {
        ui->comboBox->addItem(i.name,i.id);
    }
}

NewSubcategoryWindow::~NewSubcategoryWindow()
{
    delete ui;
}

void NewSubcategoryWindow::on_pushButton_Commit_clicked()
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
        if(m_data->addSubcategory(ui->lineEdit->text(),
                               ui->comboBox->currentData().toInt(),
                                   error_msg))
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Information);
            msg.setText(ui->lineEdit->text()+" dodano pomyślnie!");
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

void NewSubcategoryWindow::on_pushButton_Canel_clicked()
{
    close();
}
