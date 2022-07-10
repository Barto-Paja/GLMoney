#include "transatcionshistorywindow.hpp"
#include "ui_transatcionshistorywindow.h"

TransatcionsHistoryWindow::TransatcionsHistoryWindow(Data *data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransatcionsHistoryWindow),
    m_data(data)
{
    ui->setupUi(this);

    ui->dateEdit_from->setDate(QDate::currentDate());
    ui->dateEdit_from->setDate(QDate{ui->dateEdit_from->date().year(),
                                     ui->dateEdit_from->date().month(),1});

    fillCombobox();
    fillTable();

}

TransatcionsHistoryWindow::~TransatcionsHistoryWindow()
{
    delete ui;
}

bool TransatcionsHistoryWindow::fillTable()
{
    ui->tableWidget->setRowCount(0);
    m_transactions.clear();

    QString error_msg;

    QDate end = {ui->dateEdit_from->date().year(), ui->dateEdit_from->date().month(), ui->dateEdit_from->date().daysInMonth()};

    m_data->getTransactionsHistory(ui->dateEdit_from->date(),end,m_transactions,error_msg);

    ui->tableWidget->setRowCount(m_transactions.size());

    qDebug() << m_transactions.size();

    int index = 0;
    for(const auto & i : qAsConst(m_transactions))
    {
        if(currentAccountSelected != maxAccountId)
        {
            if(currentAccountSelected != i.AccountID)
            {
                continue;
            }
        }

        QTableWidgetItem *newItem;

        // ID
        newItem = new QTableWidgetItem(QString("%1").arg(i.ID));
        ui->tableWidget->setItem(index,0,newItem);
        // Data
        newItem = new QTableWidgetItem(i.Date.toString("yyyy-MM-dd"));
        ui->tableWidget->setItem(index,1,newItem);
        // Konto
        newItem = new QTableWidgetItem(i.Account);
        ui->tableWidget->setItem(index,2,newItem);
        // Płatnik
        newItem = new QTableWidgetItem(i.Payee);
        ui->tableWidget->setItem(index,3,newItem);
        // Kwota
        newItem = new QTableWidgetItem(QString("%1").arg(i.Amount));
        ui->tableWidget->setItem(index,4,newItem);
        // Centrum kosztów
        newItem = new QTableWidgetItem(i.Member);
        ui->tableWidget->setItem(index,5,newItem);
        // Kategoria
        newItem = new QTableWidgetItem(i.Category + " - " + i.Subcategory );
        ui->tableWidget->setItem(index,6,newItem);
        // Notatka
        newItem = new QTableWidgetItem(i.Note);
        ui->tableWidget->setItem(index,7,newItem);

        index++;
    }
    ui->tableWidget->setRowCount(index);
    ui->tableWidget->update();

    return true;
}

void TransatcionsHistoryWindow::fillCombobox()
{
    auto v { 0 };

    for(const auto & account : m_data->accounts())
    {
        ui->comboBox_accounts->addItem(account.Name,account.ID);
        v = account.ID;
    }

    ui->comboBox_accounts->addItem("Wszystkie",v+1);
    maxAccountId = v+1;
}

void TransatcionsHistoryWindow::on_dateEdit_from_userDateChanged(const QDate &date)
{
    Q_UNUSED(date);

    ui->tableWidget->setRowCount(0);
    m_transactions.clear();
    fillTable();
}

void TransatcionsHistoryWindow::on_comboBox_accounts_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    currentAccountSelected = ui->comboBox_accounts->currentData().toInt();
    fillTable();
}
