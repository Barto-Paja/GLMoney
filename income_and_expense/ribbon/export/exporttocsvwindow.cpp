#include "exporttocsvwindow.hpp"
#include "ui_exporttocsvwindow.h"

#include <QProgressDialog>
#include <QMessageBox>
#include <QThread>

ExportToCSVWindow::ExportToCSVWindow(Data *data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportToCSVWindow),
    m_data{data}
{
    ui->setupUi(this);

    ui->dateEdit_begin->setDate(QDate::currentDate());
    ui->dateEdit_end->setDate(QDate::currentDate());
    ui->progressBar->setHidden(true);
}

ExportToCSVWindow::~ExportToCSVWindow()
{
    delete ui;
}

void ExportToCSVWindow::on_lineEdit_fileName_textChanged(const QString &arg1)
{
    if(arg1.length())
    {
        ui->pushButton_fileDialog->setEnabled(true);
        ui->pushButton_generate->setEnabled(true);
    }
    else
    {
        ui->pushButton_fileDialog->setEnabled(false);
        ui->pushButton_generate->setEnabled(false);
    }
}

void ExportToCSVWindow::on_pushButton_fileDialog_clicked()
{
    m_path = QFileDialog::getExistingDirectory();
    ui->lineEdit_exportPath->setText(m_path);
    ui->pushButton_generate->setEnabled(true);
}

void ExportToCSVWindow::on_pushButton_generate_clicked()
{
    exportToCSV();
}

bool ExportToCSVWindow::exportToCSV()
{
    ui->progressBar->setHidden(false);
    QFile file { m_path + "/" + ui->lineEdit_fileName->text() + ".csv" };
    if(file.open(QIODevice::WriteOnly))
    {
        QString error_msg;
        if(m_data->getTransactionsHistory(ui->dateEdit_begin->date(),
                                          ui->dateEdit_end->date(),
                                          m_transactions,error_msg))
        {
            QTextStream stream{&file};
            stream.setEncoding(QStringConverter::Utf8);
            QLocale locale { QLocale::Polish };
            int index = 1;
            for(auto & i : m_transactions)
            {
                ui->progressBar->setValue((index/m_transactions.size())*100);
                ui->progressBar->update();

                if(!ui->checkBox_expenses->isChecked())
                {
                    if(i.Amount < 0.00)
                    {
                        index++;
                        continue;
                    }
                }
                if(!ui->checkBox_income->isChecked())
                {
                    if(i.Amount >= 0.00)
                    {
                        index++;
                        continue;
                    }
                }

                stream << i.Account << ";" << i.Payee << ";" << i.Member << ";"
                << i.Date.toString("yyyy-MM-dd") << ";" << locale.toString(i.Amount,'f',2) << ";"
                << i.Category << ";" << i.Subcategory << ";" << "\n";
                index++;
            }
        }

    }
    file.close();

    QMessageBox msg;
    msg.setIcon(QMessageBox::Information);
    msg.setText("Wyeksportowano pomyślnie!");
    msg.addButton(QMessageBox::Ok);
    msg.exec();

    ui->progressBar->setHidden(true);

    return true;
}
