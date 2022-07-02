#include "homebudget_form.hpp"
#include "ui_homebudget_form.h"

#include <QHeaderView>
#include <QLocale>
#include "monthresume.hpp"

HomeBudget_Form::HomeBudget_Form(Data *data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeBudget_Form),
    m_data { data }
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    emit on_dateEdit_dateChanged(ui->dateEdit->date());

    ui->tableWidget_fixedExpenses->setColumnWidth(0,240);
    ui->tableWidget_oneOffExpenses->setColumnWidth(0,240);

    ui->lineEdit_amount_fixedExpense->setValidator(new QDoubleValidator(0,1000000.00,2,this));
    ui->lineEdit_amount_oneOffExpense->setValidator(new QDoubleValidator(0,1000000.00,2,this));
    ui->lineEdit_income->setValidator(new QDoubleValidator(0,1000000.00,2,this));

    for(const auto & category : m_data->categories())
    {
        ui->comboBox_category->addItem(category.Name,category.ID);
        ui->comboBox_categoryOneOffExpense->addItem(category.Name,category.ID);
    }
}

HomeBudget_Form::~HomeBudget_Form()
{
    delete ui;
}

void HomeBudget_Form::on_pushButton_remove_fixedExpense_clicked()
{
    if(ui->tableWidget_fixedExpenses->currentRow() < 0)
    {
        return;
    }

    if(ui->tableWidget_fixedExpenses->currentRow() == 0)
    {
        m_fixedExpanses.pop_back();
    }
    else
    {
        m_fixedExpanses.erase(m_fixedExpanses.begin()+ui->tableWidget_fixedExpenses->currentRow());
    }
    ui->tableWidget_fixedExpenses->removeRow(ui->tableWidget_fixedExpenses->currentRow());
    calulate();
}

void HomeBudget_Form::on_pushButton_insert_fixedExpense_clicked()
{
    QTableWidget * src = ui->tableWidget_fixedExpenses;

    src->insertRow(src->rowCount());
    src->setItem(src->rowCount()-1,0,new QTableWidgetItem(ui->comboBox_category->currentText()));
    src->setItem(src->rowCount()-1,1,new QTableWidgetItem(ui->lineEdit_amount_fixedExpense->text()));

    m_fixedExpanses.append(FixedExpanseCalculation{
                               0,0,ui->comboBox_category->currentData().toInt(),
                               QLocale::system().toDouble(ui->lineEdit_amount_fixedExpense->text()),
                               ui->comboBox_category->currentText()
                           });
    calulate();
}

void HomeBudget_Form::on_pushButton_remove_Expense_clicked()
{
    if(ui->tableWidget_oneOffExpenses->currentRow() < 0)
    {
        return;
    }

    if(ui->tableWidget_oneOffExpenses->currentRow() == 0)
    {
        m_oneoffExpanses.pop_back();
    }
    else
    {
        m_oneoffExpanses.erase(m_oneoffExpanses.begin()+ui->tableWidget_oneOffExpenses->currentRow());
    }
    ui->tableWidget_oneOffExpenses->removeRow(ui->tableWidget_oneOffExpenses->currentRow());
    calulate();
}

void HomeBudget_Form::on_pushButton_insert_Expense_clicked()
{
    QTableWidget * src = ui->tableWidget_oneOffExpenses;

    src->insertRow(src->rowCount());
    src->setItem(src->rowCount()-1,0,new QTableWidgetItem(ui->lineEdit_expanseName->text()));
    src->setItem(src->rowCount()-1,1,new QTableWidgetItem(ui->lineEdit_amount_oneOffExpense->text()));
    src->setItem(src->rowCount()-1,2,new QTableWidgetItem(ui->comboBox_categoryOneOffExpense->currentText()));

    m_oneoffExpanses.append(OneOffExapanseCalculation{
                               0,0,
                               QLocale::system().toDouble(ui->lineEdit_amount_oneOffExpense->text()),
                               ui->lineEdit_expanseName->text(),
                               ui->comboBox_categoryOneOffExpense->currentData().toInt()
                           });

    calulate();
}

void HomeBudget_Form::on_pushButton_canel_clicked()
{
    emit back();
}

void HomeBudget_Form::on_pushButton_commit_clicked()
{
    QMessageBox msg;
    msg.setIcon(QMessageBox::Warning);
    msg.setText("Czy chcesz zakończyć szacowanie budżetu na ten miesiąc?\nPo wysłaniu nie będzie możliwa zmiana.");
    msg.addButton(QMessageBox::Yes);
    msg.addButton(QMessageBox::No);
    if(msg.exec() == QMessageBox::Yes)
    {
       if( m_data->commitHomeBudgetCalculation(m_hbc,m_fixedExpanses,m_oneoffExpanses,m_lastError))
       {
           QMessageBox msg;
           msg.setIcon(QMessageBox::Information);
           msg.setText("Transakcja dodana pomyślnie!");
           msg.addButton(QMessageBox::Ok);
           msg.exec();

           on_dateEdit_dateChanged(ui->dateEdit->date());
       }
       else
       {
           QMessageBox msg;
           msg.setIcon(QMessageBox::Critical);
           msg.setText("Nieduana transakcja!\n"+m_lastError);
           msg.addButton(QMessageBox::Ok);
           msg.exec();
       }
    }

    return;
}

void HomeBudget_Form::calulate()
{
    m_hbc.date = ui->dateEdit->date();
    m_hbc.declaredIncome = QLocale::system().toDouble(ui->lineEdit_income->text());

    auto expanses { 0.00 };
    for(auto i : m_fixedExpanses )
    {
        expanses += i.amount;
    }
    for(auto i : m_oneoffExpanses)
    {
        expanses += i.amount;
    }

    auto result = QLocale::system().toDouble(ui->lineEdit_income->text()) - expanses;

    ui->lineEdit_resume->setText(QString::number(result));

    if(result >= 0)
    {
        ui->label_message->setText("Saldo jest dodatnie, brawo! "+QString::number(result) + " zł możesz zaoszczędzić.");
    }
    else
    {
        ui->label_message->setText("Saldo jest ujemne! O "+QString::number((-1)*result) + " zł musisz uszczuplić swoje oszczędności.");
    }

}

void HomeBudget_Form::on_lineEdit_income_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    calulate();
}

void HomeBudget_Form::on_dateEdit_dateChanged(const QDate &date)
{
    if(!m_data->checkIsHomeBudgetCalculationExist(date,m_hbc,m_lastError))
    {

    }

    ui->lineEdit_income->setText(QString::number(m_hbc.declaredIncome));

    ui->tableWidget_fixedExpenses->setRowCount(0);
    ui->tableWidget_oneOffExpenses->setRowCount(0);
    m_fixedExpanses.clear();
    m_oneoffExpanses.clear();

    if(m_hbc.isValid)
    {
        m_data->loadHomeBudgetExpanses(m_hbc,m_fixedExpanses,m_oneoffExpanses,m_lastError);

        ui->tableWidget_fixedExpenses->setRowCount(m_fixedExpanses.size());
        ui->tableWidget_oneOffExpenses->setRowCount(m_oneoffExpanses.size());

        QTableWidget * feT = ui->tableWidget_fixedExpenses;
        QTableWidget * ofT = ui->tableWidget_oneOffExpenses;

        int index = 0;
        for(auto fe : m_fixedExpanses)
        {
            QTableWidgetItem *newItem;

            newItem = new QTableWidgetItem{fe.category};
            feT->setItem(index,0,newItem);

            newItem = new QTableWidgetItem{QString::number(fe.amount)};
            feT->setItem(index,1,newItem);

            index++;
        }

        index = 0;
        for(auto of : m_oneoffExpanses)
        {
            QTableWidgetItem *newItem;

            newItem = new QTableWidgetItem{of.name};
            ofT->setItem(index,0,newItem);

            newItem = new QTableWidgetItem{QString::number(of.amount)};
            ofT->setItem(index,1,newItem);

            newItem = new QTableWidgetItem{of.category_name};
            ofT->setItem(index,2,newItem);

            index++;
        }

        ui->pushButton_commit->setEnabled(false);
        ui->pushButton_insert_Expense->setEnabled(false);
        ui->pushButton_insert_fixedExpense->setEnabled(false);
        ui->pushButton_remove_Expense->setEnabled(false);
        ui->pushButton_remove_fixedExpense->setEnabled(false);
    }
    else
    {
        ui->pushButton_commit->setEnabled(true);
        ui->pushButton_insert_Expense->setEnabled(true);
        ui->pushButton_insert_fixedExpense->setEnabled(true);
        ui->pushButton_remove_Expense->setEnabled(true);
        ui->pushButton_remove_fixedExpense->setEnabled(true);
    }

    calulate();
}

void HomeBudget_Form::on_pushButton_back_clicked()
{
    emit back();
}

void HomeBudget_Form::on_pushButton_Resume_clicked()
{
    MonthResume * dialog = new MonthResume(m_data,ui->dateEdit->date(),this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}
