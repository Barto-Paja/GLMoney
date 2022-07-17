#include "incomesandexpenses_form.hpp"
#include "income_and_expense/ribbon/core/addnewcategorywindow.hpp"
#include "income_and_expense/ribbon/core/newaccountwindow.hpp"
#include "income_and_expense/ribbon/core/newmemberwindow.hpp"
#include "income_and_expense/ribbon/core/newpayeewindow.hpp"
#include "income_and_expense/ribbon/core/newsubcategorywindow.hpp"
#include "income_and_expense/ribbon/core/payeeinformationwindow.hpp"
#include "ui_incomesandexpenses_form.h"

IncomesAndExpenses_Form::IncomesAndExpenses_Form(Data *data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IncomesAndExpenses_Form),
    m_data { data }
{
    ui->setupUi(this);

    ui->calendarWidget->hide();
    ui->lineEdit_amount->setStyleSheet("background-color: #F0685B; color: #57201B");
    ui->dateEdit->setDate(QDate::currentDate());
    ui->lineEdit_amount->setValidator(new QDoubleValidator(0,1000000.00,2,this));

    FillComboboxes();
}

IncomesAndExpenses_Form::~IncomesAndExpenses_Form()
{
    delete ui;
}

void IncomesAndExpenses_Form::on_comboBox_category_currentIndexChanged(int index)
{
    if(ui->comboBox_category->count() != 0)
    {
        ReloadSubcategories(index);
    }
}

void IncomesAndExpenses_Form::on_calendarWidget_clicked(const QDate &date)
{
    ui->dateEdit->setDate(date);
}

void IncomesAndExpenses_Form::on_checkBox_isIncome_stateChanged(int arg1)
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

void IncomesAndExpenses_Form::on_pushButton_Commit_clicked()
{

    if(ui->lineEdit_amount->text().isEmpty())
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setText("Pole z kwotą nie może być puste!");
        msg.addButton(QMessageBox::Ok);
        msg.exec();

        return;
    }

    QString error_msg;

    if(ui->checkBox_isIncome->isChecked())
    {
        if(m_data->addTransaction(newTransaction{
                                      ui->dateEdit->date(),
                                      ui->comboBox_payee->currentData().toInt(),
                                      QLocale::system().toDouble(ui->lineEdit_amount->text()),
                                      ui->comboBox_subcategory->currentData().toInt(),
                                      ui->comboBox_member->currentData().toInt(),
                                      ui->comboBox_account->currentData().toInt(),
                                      ui->textEdit->toPlainText(),
                                      1
                                  }, error_msg))
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Information);
            msg.setText("Transakcja dodana pomyślnie!");
            msg.addButton(QMessageBox::Ok);
            msg.exec();

            ui->textEdit->clear();
        }
        else
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Critical);
            msg.setText("Dodanie transakcji nie powiodło się!\n"+error_msg);
            msg.addButton(QMessageBox::Ok);
            msg.exec();
        }
    }
    else
    {
        if(m_data->addTransaction(
                newTransaction
                {
                    ui->dateEdit->date(),
                    ui->comboBox_payee->currentData().toInt(),
                    QLocale::system().toDouble(ui->lineEdit_amount->text()),
                    ui->comboBox_subcategory->currentData().toInt(),
                    ui->comboBox_member->currentData().toInt(),
                    ui->comboBox_account->currentData().toInt(),
                    ui->textEdit->toPlainText(),
                    -1
                }, error_msg))
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Information);
            msg.setText("Transakcja dodana pomyślnie!");
            msg.addButton(QMessageBox::Ok);
            msg.exec();

            ui->textEdit->clear();
        }
        else
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Critical);
            msg.setText("Dodanie transakcji nie powiodło się!\n"+error_msg);
            msg.addButton(QMessageBox::Ok);
            msg.exec();
        }
    }
}

void IncomesAndExpenses_Form::on_pushButton_Back_clicked()
{
    emit back();
}

void IncomesAndExpenses_Form::on_pushButton_dateSelect_clicked()
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

// -- Tab Widget

void IncomesAndExpenses_Form::on_toolButton_addCategory_clicked()
{
    AddNewCategoryWindow * dialog = new AddNewCategoryWindow(m_data);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &AddNewCategoryWindow::destroyed, this, [&](){ReloadCategories();});
    dialog->show();
}

void IncomesAndExpenses_Form::on_toolButton_addSubcategory_clicked()
{
    NewSubcategoryWindow * dialog = new NewSubcategoryWindow(m_data);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &NewSubcategoryWindow::destroyed, this, [&](){ReloadCategories();});
    dialog->show();
}

void IncomesAndExpenses_Form::on_toolButton_addAccount_clicked()
{
    NewAccountWindow * dialog = new NewAccountWindow(m_data);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &NewPayeeWindow::destroyed, this, [&](){ReloadAccounts();});
    dialog->show();
}

void IncomesAndExpenses_Form::on_toolButton_addPayee_clicked()
{
    NewPayeeWindow * dialog = new NewPayeeWindow(m_data);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &NewPayeeWindow::destroyed, this, [&](){ReloadPayees();});
    dialog->show();
}

void IncomesAndExpenses_Form::on_toolButton_addMember_clicked()
{
    NewMemberWindow * dialog = new NewMemberWindow(m_data);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &NewPayeeWindow::destroyed, this, [&](){ReloadMembers();});
    dialog->show();
}

void IncomesAndExpenses_Form::on_toolButton_reportMonth_clicked()
{
    TransatcionsHistoryWindow * dialog = new TransatcionsHistoryWindow(m_data);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void IncomesAndExpenses_Form::on_toolButton_makeTransactionsBetweenAccounts_clicked()
{
    MakeTransactionsBetweenAccounts * dialog = new MakeTransactionsBetweenAccounts(m_data);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void IncomesAndExpenses_Form::on_toolButton_exportToCSV_clicked()
{
    ExportToCSVWindow * dialog = new ExportToCSVWindow(m_data);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

// -- Tab Widget

void IncomesAndExpenses_Form::FillComboboxes()
{
    for(const auto & account : m_data->accounts())
    {
        ui->comboBox_account->addItem(account.Name,account.ID);
    }

    for(const auto & member : m_data->members())
    {
        ui->comboBox_member->addItem(member.Name,member.ID);
    }

    for(const auto & payee : m_data->payees())
    {
        ui->comboBox_payee->addItem(payee.Name,payee.ID);
    }

    for(const auto & category : m_data->categories())
    {
        ui->comboBox_category->addItem(category.Name,category.ID);
    }
}

void IncomesAndExpenses_Form::ReloadSubcategories(int index)
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

void IncomesAndExpenses_Form::ReloadPayees()
{
    ui->comboBox_payee->clear();

    m_data->reloadPayee();

    for(const auto & payee : m_data->payees())
    {
        ui->comboBox_payee->addItem(payee.Name,payee.ID);
    }

    ui->comboBox_payee->update();
}

void IncomesAndExpenses_Form::ReloadCategories()
{
    ui->comboBox_subcategory->clear();
    ui->comboBox_category->clear();

    m_data->reloadCategory();

    for(const auto & category : m_data->categories())
    {
        ui->comboBox_category->addItem(category.Name,category.ID);
    }

    ui->comboBox_category->update();
}

void IncomesAndExpenses_Form::ReloadAccounts()
{
    ui->comboBox_account->clear();

    m_data->reloadAccount();

    for(const auto & account : m_data->accounts())
    {
        ui->comboBox_account->addItem(account.Name,account.ID);
    }

    ui->comboBox_account->update();
}

void IncomesAndExpenses_Form::ReloadMembers()
{
    ui->comboBox_member->clear();

    m_data->reloadMember();

    for(const auto & member : m_data->members())
    {
        ui->comboBox_member->addItem(member.Name,member.ID);
    }

    ui->comboBox_member->update();
}

void IncomesAndExpenses_Form::on_toolButton_ImportTransactions_clicked()
{
    ImportCSVWindow * dialog = new ImportCSVWindow(m_data,this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void IncomesAndExpenses_Form::on_pushButton_editPayee_clicked()
{
    auto i = ui->comboBox_payee->currentData().toInt();
    PayeeInformationWindow * dialog = new PayeeInformationWindow(i,m_data,this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

