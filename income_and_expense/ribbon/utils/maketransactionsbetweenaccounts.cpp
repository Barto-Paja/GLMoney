#include "maketransactionsbetweenaccounts.hpp"
#include "ui_maketransactionsbetweenaccounts.h"

MakeTransactionsBetweenAccounts::MakeTransactionsBetweenAccounts(Data *data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MakeTransactionsBetweenAccounts),
    m_data(data)
{
    ui->setupUi(this);

    ui->lineEdit->setValidator(new QDoubleValidator(0,1000000.00,2,this));

    for(auto & i : m_data->accounts())
    {
        ui->comboBox_source->addItem(i.name,i.id);
        ui->comboBox_target->addItem(i.name,i.id);
    }

    for(auto & i : m_data->payees())
    {
        ui->comboBox_source_payee->addItem(i.name,i.id);
        ui->comboBox_target_payee->addItem(i.name,i.id);
    }

    for(auto & i : m_data->members())
    {
        ui->comboBox_member->addItem(i.name,i.id);
    }

    for(auto & i : m_data->categories())
    {
        ui->comboBox_source_category->addItem(i.name,i.id);
        ui->comboBox_target_category->addItem(i.name,i.id);
    }
}

MakeTransactionsBetweenAccounts::~MakeTransactionsBetweenAccounts()
{
    delete ui;
}

void MakeTransactionsBetweenAccounts::on_comboBox_target_category_currentIndexChanged(int index)
{
    if(ui->comboBox_target_category->count() != 0)
    {
        reloadSubcategories(SRC::TARGET, index);
    }
}

void MakeTransactionsBetweenAccounts::reloadSubcategories(SRC src, int index)
{
    switch(src)
    {
    case SRC::SOURCE:
    {
        ui->comboBox_source_subcategory->clear();

        if(m_data->subCategories().size() > 0)
        {
            for(auto i : m_data->subCategories(index+1))
            {
                ui->comboBox_source_subcategory->addItem(i.name,i.id);
            }

            ui->comboBox_source_subcategory->update();
        }

        break;
    }
    case SRC::TARGET:
    {
        ui->comboBox_target_subcategory->clear();

        if(m_data->subCategories().size() > 0)
        {
            for(auto i : m_data->subCategories(index+1))
            {
                ui->comboBox_target_subcategory->addItem(i.name,i.id);
            }

            ui->comboBox_target_subcategory->update();
        }

        break;
    }
    }
}

void MakeTransactionsBetweenAccounts::on_comboBox_source_category_currentIndexChanged(int index)
{
    if(ui->comboBox_source_category->count() != 0)
    {
        reloadSubcategories(SRC::SOURCE, index);
    }
}

void MakeTransactionsBetweenAccounts::on_calendarWidget_clicked(const QDate &date)
{
    ui->label_date->setText(date.toString("yyyy-MM-dd"));
}

void MakeTransactionsBetweenAccounts::on_pushButton_Commit_clicked()
{
    QString error;
    TransferBetweenAccounts t;

    t.sourceID = ui->comboBox_source->currentData().toInt();
    t.sourcePayeeMirror = ui->comboBox_target_payee->currentData().toInt();
    t.sourceSubcategory = ui->comboBox_source_subcategory->currentData().toInt();

    t.targetID = ui->comboBox_target->currentData().toInt();
    t.targetPayeeMirror = ui->comboBox_source_payee->currentData().toInt();
    t.targetSubcategory = ui->comboBox_target_subcategory->currentData().toInt();

    t.date = ui->calendarWidget->selectedDate();
    t.amount = QLocale::system().toDouble(ui->lineEdit->text());
    t.memberID = ui->comboBox_member->currentData().toInt();

    if(t.sourceID == t.targetID)
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setText("Nie można zrobić transakcji pomiędzy jednym kontem!");
        msg.addButton(QMessageBox::Ok);
        msg.exec();

        return;
    }

    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setText("Pole z kwotą nie może być puste!");
        msg.addButton(QMessageBox::Ok);
        msg.exec();

        return;
    }

    if(m_data->makeTransactionBetweenAccounts(t,error))
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Information);
        msg.setText("Transakcje dodano pomyślnie!");
        msg.addButton(QMessageBox::Ok);
        msg.exec();
    }
    else
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setText("Dodanie transakcji nie powiodło się!\n"+error);
        msg.addButton(QMessageBox::Ok);
        msg.exec();
    }
}

void MakeTransactionsBetweenAccounts::on_pushButton_Canel_clicked()
{
    this->close();
}
