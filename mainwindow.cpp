#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QLocale>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->calendarWidget->hide();
    ui->lineEdit_amount->setStyleSheet("background-color: #F0685B; color: #57201B");
    ui->dateEdit->setDate(QDate::currentDate());
    ui->lineEdit_amount->setValidator(new QDoubleValidator(0,1000000.00,2,this));

    fillComboboxes();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillComboboxes()
{
    for(auto i : m_data.accounts())
    {
        ui->comboBox_account->addItem(i.name,i.id);
    }

    for(auto i : m_data.members())
    {
        ui->comboBox_member->addItem(i.name,i.id);
    }

    for(auto i : m_data.payees())
    {
        ui->comboBox_payee->addItem(i.name,i.id);
    }

    for(auto i : m_data.categories())
    {
        ui->comboBox_category->addItem(i.name,i.id);
    }
}

void MainWindow::reloadSubcategories(int index)
{
    ui->comboBox_subcategory->clear();

    if(m_data.subCategories().size() > 0)
    {
        for(auto i : m_data.subCategories(index+1))
        {
            ui->comboBox_subcategory->addItem(i.name,i.id);
        }

        ui->comboBox_subcategory->update();
    }

}


void MainWindow::on_comboBox_category_currentIndexChanged(int index)
{
    reloadSubcategories(index);
}

void MainWindow::on_pushButton_4_clicked()
{
    if(ui->calendarWidget->isHidden())
    {
        ui->calendarWidget->show();
    }
    else
    {
        ui->calendarWidget->hide();
    }
}

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    ui->dateEdit->setDate(date);
}

void MainWindow::on_checkBox_isIncome_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    if(ui->checkBox_isIncome->isChecked())
    {
        ui->lineEdit_amount->setStyleSheet("background-color: #49F249; color: #227022");
    }
    else
    {
        ui->lineEdit_amount->setStyleSheet("background-color: #F0685B; color: #57201B");
    }
}

void MainWindow::on_pushButton_Commit_clicked()
{

    if(ui->checkBox_isIncome->isChecked())
    {
        m_data.addTransaction(newTransaction{
            ui->dateEdit->date(),
            ui->comboBox_payee->currentData().toInt(),
            QLocale::system().toDouble(ui->lineEdit_amount->text()),
            ui->comboBox_subcategory->currentData().toInt(),
            ui->comboBox_member->currentData().toInt(),
            ui->comboBox_account->currentData().toInt(),
            ui->textEdit->toPlainText(),
            1
        });
    }
    else
    {
        m_data.addTransaction(newTransaction{
            ui->dateEdit->date(),
            ui->comboBox_payee->currentData().toInt(),
            QLocale::system().toDouble(ui->lineEdit_amount->text()),
            ui->comboBox_subcategory->currentData().toInt(),
            ui->comboBox_member->currentData().toInt(),
            ui->comboBox_account->currentData().toInt(),
            ui->textEdit->toPlainText(),
            -1
        });
    }
}

void MainWindow::on_pushButton_Back_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox msg;
    msg.setIcon(QMessageBox::Information);
    msg.setText("Funkcjonalnosć zostanie utworzona wkrótce.");
    msg.addButton(QMessageBox::Ok);
    msg.exec();
}

void MainWindow::on_pushButton_3_clicked()
{
    exit(0);
}
