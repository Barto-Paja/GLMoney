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

    fillTable();

}

TransatcionsHistoryWindow::~TransatcionsHistoryWindow()
{
    delete ui;
}

bool TransatcionsHistoryWindow::fillTable()
{
    QString error_msg;

    QDate end = {ui->dateEdit_from->date().year(), ui->dateEdit_from->date().month(), ui->dateEdit_from->date().daysInMonth()};

    m_data->getTransactionsHistory(ui->dateEdit_from->date(),end,m_transactions,error_msg);

    ui->tableWidget->setRowCount(m_transactions.size());

    qDebug() << m_transactions.size();

    int index = 0;
    for(auto i : m_transactions)
    {
        QTableWidgetItem *newItem;

        // ID
        newItem = new QTableWidgetItem(QString("%1").arg(i.id));
        ui->tableWidget->setItem(index,0,newItem);
        // Data
        newItem = new QTableWidgetItem(i.date.toString("yyyy-MM-dd"));
        ui->tableWidget->setItem(index,1,newItem);
        // Konto
        newItem = new QTableWidgetItem(i.account);
        ui->tableWidget->setItem(index,2,newItem);
        // Płatnik
        newItem = new QTableWidgetItem(i.payee);
        ui->tableWidget->setItem(index,3,newItem);
        // Kwota
        newItem = new QTableWidgetItem(QString("%1").arg(i.amount));
        ui->tableWidget->setItem(index,4,newItem);
        // Centrum kosztów
        newItem = new QTableWidgetItem(i.member);
        ui->tableWidget->setItem(index,5,newItem);
        // Kategoria
        newItem = new QTableWidgetItem(i.category + " - " + i.subcategory );
        ui->tableWidget->setItem(index,6,newItem);
        // Notatka
        newItem = new QTableWidgetItem(i.note);
        ui->tableWidget->setItem(index,7,newItem);

        index++;
    }

    ui->tableWidget->update();

    return true;
}

void TransatcionsHistoryWindow::on_dateEdit_from_userDateChanged(const QDate &date)
{
    Q_UNUSED(date);

    ui->tableWidget->setRowCount(0);
    m_transactions.clear();
    fillTable();
}
