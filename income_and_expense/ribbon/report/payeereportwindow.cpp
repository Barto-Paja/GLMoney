#include "payeereportwindow.hpp"
#include "ui_payeereportwindow.h"

#include <QMessageBox>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLocale>
#include <QDate>

PayeeReportWindow::PayeeReportWindow(Data *data, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PayeeReportWindow)
    , m_data(data)
{
    ui->setupUi(this);

    setWindowTitle("Raport kontrahenta");

    // Ustawienia dat
    QDate today = QDate::currentDate();

    ui->dateEditFrom->setCalendarPopup(true);
    ui->dateEditTo->setCalendarPopup(true);

    ui->dateEditFrom->setDisplayFormat("dd.MM.yyyy");
    ui->dateEditTo->setDisplayFormat("dd.MM.yyyy");

    ui->dateEditFrom->setDate(QDate(today.year(), today.month(), 1));
    ui->dateEditTo->setDate(today);

    // Tabela
    setupTable();

    // Kontrahenci
    loadPayees();

    // Podsumowanie
    updateSummary(0.0, 0.0, 0.0);

    // Obsługa przycisku
    connect(ui->pushButtonGenerate,
            &QPushButton::clicked,
            this,
            &PayeeReportWindow::generateReport);
}

PayeeReportWindow::~PayeeReportWindow()
{
    delete ui;
}

void PayeeReportWindow::setupTable()
{
    ui->tableWidgetReport->setColumnCount(4);

    QStringList headers;
    headers << "Miesiąc"
            << "Przychody"
            << "Wydatki"
            << "Saldo";

    ui->tableWidgetReport->setHorizontalHeaderLabels(headers);

    ui->tableWidgetReport->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);

    ui->tableWidgetReport->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    ui->tableWidgetReport->setSelectionBehavior(
        QAbstractItemView::SelectRows);

    ui->tableWidgetReport->setSelectionMode(
        QAbstractItemView::SingleSelection);

    ui->tableWidgetReport->setAlternatingRowColors(true);

    ui->tableWidgetReport->setRowCount(0);
}

void PayeeReportWindow::loadPayees()
{
    ui->comboBoxPayee->clear();

    if (!m_data) {
        return;
    }

    const QVector<Payee> payees = m_data->payees();

    for (const Payee &payee : payees) {
        ui->comboBoxPayee->addItem(payee.Name, payee.ID);
    }
}

void PayeeReportWindow::updateSummary(
    double income,
    double expenses,
    double balance)
{
    QLocale locale(QLocale::Polish, QLocale::Poland);

    ui->labelIncome->setText(
        "Przychody: " + locale.toString(income, 'f', 2) + " zł");

    ui->labelExpenses->setText(
        "Wydatki: " + locale.toString(expenses, 'f', 2) + " zł");

    ui->labelBalance->setText(
        "Saldo: " + locale.toString(balance, 'f', 2) + " zł");
}

void PayeeReportWindow::generateReport()
{
    if (!m_data) {
        QMessageBox::critical(
            this,
            "Błąd",
            "Brak dostępu do danych.");
        return;
    }

    if (ui->comboBoxPayee->currentIndex() < 0) {
        QMessageBox::warning(
            this,
            "Brak kontrahenta",
            "Wybierz kontrahenta.");
        return;
    }

    const QDate start = ui->dateEditFrom->date();
    const QDate end = ui->dateEditTo->date();

    if (start > end) {
        QMessageBox::warning(
            this,
            "Nieprawidłowy zakres",
            "Data początkowa nie może być późniejsza od końcowej.");
        return;
    }

    const int payeeID =
        ui->comboBoxPayee->currentData().toInt();

    QVector<PayeeMonthlyReport> report;
    QString error;

    if (!m_data->getPayeeMonthlyReport(
            payeeID,
            start,
            end,
            report,
            error)) {

        QMessageBox::critical(
            this,
            "Błąd raportu",
            "Nie udało się wygenerować raportu:\n" + error);

        return;
    }

    ui->tableWidgetReport->setRowCount(0);

    double totalIncome = 0.0;
    double totalExpenses = 0.0;
    double totalBalance = 0.0;

    QLocale locale(QLocale::Polish, QLocale::Poland);

    for (const PayeeMonthlyReport &item : report) {

        const int row = ui->tableWidgetReport->rowCount();

        ui->tableWidgetReport->insertRow(row);

        const QString month =
            QString("%1.%2")
                .arg(item.month, 2, 10, QChar('0'))
                .arg(item.year);

        const QString income =
            locale.toString(item.income, 'f', 2);

        const QString expenses =
            locale.toString(item.expenses, 'f', 2);

        const QString balance =
            locale.toString(item.balance, 'f', 2);

        ui->tableWidgetReport->setItem(
            row, 0, new QTableWidgetItem(month));

        ui->tableWidgetReport->setItem(
            row, 1, new QTableWidgetItem(income));

        ui->tableWidgetReport->setItem(
            row, 2, new QTableWidgetItem(expenses));

        ui->tableWidgetReport->setItem(
            row, 3, new QTableWidgetItem(balance));

        totalIncome += item.income;
        totalExpenses += item.expenses;
        totalBalance += item.balance;
    }

    updateSummary(
        totalIncome,
        totalExpenses,
        totalBalance
    );

    ui->tableWidgetReport->resizeRowsToContents();
}

void PayeeReportWindow::on_pushButton_back_clicked()
{
    emit backToAnalytics();
}

