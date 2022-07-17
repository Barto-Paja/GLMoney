#include "payeeinformationwindow.hpp"
#include <QMessageBox>
#include "ui_payeeinformationwindow.h"

PayeeInformationWindow::PayeeInformationWindow(const int & payee_id, Data *data, QWidget *parent) :
    QDialog(parent),
    ui{new Ui::PayeeInformationWindow},
    m_data{data},
    m_payeeID{payee_id}
{
    ui->setupUi(this);

    LoadCategories();
    LoadPayee();
}

PayeeInformationWindow::~PayeeInformationWindow()
{
    delete ui;
}

bool PayeeInformationWindow::LoadCategories()
{
    for(const auto & category : m_data->categories())
    {
        ui->comboBox_category->addItem(category.Name,category.ID);
    }

    return true;
}

void PayeeInformationWindow::ReloadSubcategories(int index)
{
    ui->comboBox_subcategory->clear();

    if(!m_data->subCategories().isEmpty())
    {
        for(const auto & subcategory : m_data->subCategories(index+1))
        {
            ui->comboBox_subcategory->addItem(subcategory.Name,subcategory.ID);
        }

        ui->comboBox_subcategory->update();
    }
}

bool PayeeInformationWindow::LoadPayee()
{
    data  = m_data->GetPayee(m_payeeID);
    ui->lineEdit->setText(data.Name);

    auto sb { m_data->GetSubCategory(data.SuggestedSubCategoryID)};

    int index = ui->comboBox_category->findData(sb.RootID);
    if(index != -1)
    {
        ui->comboBox_category->setCurrentIndex(index);
        index = ui->comboBox_subcategory->findData(sb.ID);
        if(index != -1)
        {
            ui->comboBox_subcategory->setCurrentIndex(index);
        }
    }

    return true;
}

void PayeeInformationWindow::on_comboBox_category_currentIndexChanged(int index)
{
    if(ui->comboBox_category->count() != 0)
    {
        ReloadSubcategories(index);
    }
}


void PayeeInformationWindow::on_pushButton_save_clicked()
{
    if(m_data->UpdatePayee(Payee{
                           data.ID,
                           data.Name,
                           ui->comboBox_subcategory->currentData().toInt(),
                           ui->comboBox_subcategory->currentText()}
                           ))
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Information);
        msg.setText("Transakcja dodana pomyślnie!");
        msg.addButton(QMessageBox::Ok);
        msg.exec();
    }
}

