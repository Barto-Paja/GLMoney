#include "incomesandexpenses_form.hpp"
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

    fillComboboxes();
}

IncomesAndExpenses_Form::~IncomesAndExpenses_Form()
{
    delete ui;
}

void IncomesAndExpenses_Form::on_comboBox_category_currentIndexChanged(int index)
{
    if(ui->comboBox_category->count() != 0)
    {
        reloadSubcategories(index);
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
    connect(dialog, &AddNewCategoryWindow::destroyed, this, [&](){reloadCategories();});
    dialog->show();
}

void IncomesAndExpenses_Form::on_toolButton_addSubcategory_clicked()
{
    NewSubcategoryWindow * dialog = new NewSubcategoryWindow(m_data);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &NewSubcategoryWindow::destroyed, this, [&](){reloadCategories();});
    dialog->show();
}

void IncomesAndExpenses_Form::on_toolButton_addAccount_clicked()
{
    NewAccountWindow * dialog = new NewAccountWindow(m_data);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &NewPayeeWindow::destroyed, this, [&](){reloadAccounts();});
    dialog->show();
}

void IncomesAndExpenses_Form::on_toolButton_addPayee_clicked()
{
    NewPayeeWindow * dialog = new NewPayeeWindow(m_data);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &NewPayeeWindow::destroyed, this, [&](){reloadPayees();});
    dialog->show();
}

void IncomesAndExpenses_Form::on_toolButton_addMember_clicked()
{
    NewMemberWindow * dialog = new NewMemberWindow(m_data);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &NewPayeeWindow::destroyed, this, [&](){reloadMembers();});
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

void IncomesAndExpenses_Form::fillComboboxes()
{
    for(auto & i : m_data->accounts())
    {
        ui->comboBox_account->addItem(i.name,i.id);
    }

    for(auto & i : m_data->members())
    {
        ui->comboBox_member->addItem(i.name,i.id);
    }

    for(auto & i : m_data->payees())
    {
        ui->comboBox_payee->addItem(i.name,i.id);
    }

    for(auto & i : m_data->categories())
    {
        ui->comboBox_category->addItem(i.name,i.id);
    }
}

void IncomesAndExpenses_Form::reloadSubcategories(int index)
{
    ui->comboBox_subcategory->clear();

    if(m_data->subCategories().size() > 0)
    {
        for(auto & i : m_data->subCategories(index+1))
        {
            ui->comboBox_subcategory->addItem(i.name,i.id);
        }

        ui->comboBox_subcategory->update();
    }
}

void IncomesAndExpenses_Form::reloadPayees()
{
    ui->comboBox_payee->clear();

    m_data->reloadPayee();

    for(auto & i : m_data->payees())
    {
        ui->comboBox_payee->addItem(i.name,i.id);
    }

    ui->comboBox_payee->update();
}

void IncomesAndExpenses_Form::reloadCategories()
{
    ui->comboBox_subcategory->clear();
    ui->comboBox_category->clear();

    m_data->reloadCategory();

    for(auto & i : m_data->categories())
    {
        ui->comboBox_category->addItem(i.name,i.id);
    }

    ui->comboBox_category->update();
}

void IncomesAndExpenses_Form::reloadAccounts()
{
    ui->comboBox_account->clear();

    m_data->reloadAccount();

    for(auto & i : m_data->accounts())
    {
        ui->comboBox_account->addItem(i.name,i.id);
    }

    ui->comboBox_account->update();
}

void IncomesAndExpenses_Form::reloadMembers()
{
    ui->comboBox_member->clear();

    m_data->reloadMember();

    for(auto & i : m_data->members())
    {
        ui->comboBox_member->addItem(i.name,i.id);
    }

    ui->comboBox_member->update();
}

