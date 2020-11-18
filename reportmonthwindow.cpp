#include "reportmonthwindow.hpp"
#include "ui_reportmonthwindow.h"

#include <QLineSeries>
#include <QDateTime>
#include <QDebug>

/*!
 * \brief ReportMonthWindow::ReportMonthWindow
 * \param data
 * \param parent
 *
 *
 * TODO: Popakować do do osobnch funkcji
 * TODO: Ponazywać linie
 * TODO: Wypełnić tabelę
 *
 */

ReportMonthWindow::ReportMonthWindow(Data *data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportMonthWindow),
    m_data(data)
{
    ui->setupUi(this);

    QLineSeries * incomes = new QLineSeries();
    QLineSeries * expanses = new QLineSeries();

    QDateTimeAxis * axisX = new QDateTimeAxis();

    //axisX->setTickCount(10);
    axisX->setMin(QDateTime{QDate{2020,1,1},QTime{12,00}});
    axisX->setMax(QDateTime{QDate{2020,12,31},QTime{12,00}});
    axisX->setFormat("MMM yyyy");
    axisX->setTitleText("Date");
    //chart->addAxis(axisX, Qt::AlignBottom);

    QVector<transactionsResume> incomes_data;
    QVector<transactionsResume> expanses_data;

    QString error_msg;

    ui->dateEdit_from->setDate(QDate::currentDate());
    ui->dateEdit_to->setDate(QDate::currentDate());

    if(!m_data->getTransactionsResume(
        ui->dateEdit_from->date().year(),
        ui->dateEdit_to->date().year(),
        expanses_data,
        incomes_data,
        error_msg
            ))
    {
        return;
    }

    for(auto i : incomes_data)
    {
        QDateTime date;
        date.setDate(QDate(i.year,i.month,1));
        date.setTime(QTime{12,0});
        incomes->append(date.toMSecsSinceEpoch(),i.amount);
    }

    for(auto i : expanses_data)
    {
        QDateTime date;
        date.setDate(QDate{i.year,i.month,1});
        date.setTime(QTime{12,0});
        expanses->append(date.toMSecsSinceEpoch(),i.amount);
    }


    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0.00, 10000.5);
    axisY->setLabelFormat("%.2f");

    QChart *chart = new QChart();

    chart->addSeries(incomes);
    incomes->setName("Wpływy na konta");
    incomes->setColor(QColor{"green"});

    chart->addSeries(expanses);
    expanses->setName("Wydaki / Koszta");
    expanses->setColor(QColor{"red"});

    chart->addAxis(axisY, Qt::AlignLeft);
    chart->addAxis(axisX,Qt::AlignBottom);

    incomes->attachAxis(axisX);
    incomes->attachAxis(axisY);

    expanses->attachAxis(axisX);
    expanses->attachAxis(axisY);

    ui->graphicsView->setChart(chart);

}

ReportMonthWindow::~ReportMonthWindow()
{
    delete ui;
}
