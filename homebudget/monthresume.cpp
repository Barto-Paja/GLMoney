#include "monthresume.hpp"
#include "ui_monthresume.h"

MonthResume::MonthResume(Data *data, const QDate &date, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MonthResume),
    m_data(data),
    m_date(date)
{
    ui->setupUi(this);

    QVector<CategoryResume> resumes;
    QString error;

    m_data->getResume(m_date,resumes,error);

    ui->tableWidget->setRowCount(resumes.size());

    int index = 0;
    for(auto & i : resumes)
    {
        QTableWidgetItem * newItem;

        // Category Name
        newItem = new QTableWidgetItem(QString("%1").arg(i.CategoryName));
        ui->tableWidget->setItem(index,0,newItem);
        // Planned Expense
        newItem = new QTableWidgetItem(QString("%1").arg(i.PlannedExpense));
        ui->tableWidget->setItem(index,1,newItem);
        // Calculated Expense
        newItem = new QTableWidgetItem(QString("%1").arg(i.CalculatedExpense));
        ui->tableWidget->setItem(index,2,newItem);
        // Bilans
        newItem = new QTableWidgetItem(QString("%1").arg(i.PlannedExpense-i.CalculatedExpense));
        ui->tableWidget->setItem(index,3,newItem);

        index++;
    }

    ui->tableWidget->update();
}

MonthResume::~MonthResume()
{
    delete ui;
}
