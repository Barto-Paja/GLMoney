
#include "expensereportwindow.hpp"
#include "ui_expensereportwindow.h"

#include <QMessageBox>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDate>
#include <QLocale>
#include <QFont>

ExpenseReportWindow::ExpenseReportWindow(Data *data, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExpenseReportWindow)
    , m_data(data)
{
    ui->setupUi(this);

    setWindowTitle("Raport wydatków");

    QDate today = QDate::currentDate();

    ui->dateEditFrom->setDate(
        QDate(today.year(), today.month(), 1));

    ui->dateEditTo->setDate(today);

    setupTable();

    updateTotal(0.0);

    connect(ui->pushButtonGenerate,
            &QPushButton::clicked,
            this,
            &ExpenseReportWindow::generateReport);
}

ExpenseReportWindow::~ExpenseReportWindow()
{
    delete ui;
}

void ExpenseReportWindow::setupTable()
{
    ui->tableWidgetReport->setColumnCount(3);

    ui->tableWidgetReport->setHorizontalHeaderLabels({
        "Kategoria / podkategoria",
        "Kwota",
        "Udział"
    });

    ui->tableWidgetReport->horizontalHeader()
        ->setSectionResizeMode(0, QHeaderView::Stretch);

    ui->tableWidgetReport->horizontalHeader()
        ->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    ui->tableWidgetReport->horizontalHeader()
        ->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    ui->tableWidgetReport->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    ui->tableWidgetReport->setSelectionBehavior(
        QAbstractItemView::SelectRows);

    ui->tableWidgetReport->setSelectionMode(
        QAbstractItemView::SingleSelection);

    ui->tableWidgetReport->setAlternatingRowColors(true);

    ui->tableWidgetReport->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidgetReport->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableWidgetReport->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void ExpenseReportWindow::updateTotal(double total)
{
    QLocale locale(QLocale::Polish, QLocale::Poland);

    ui->labelTotalExpenses->setText(
        "Łączne wydatki: " +
        locale.toString(total, 'f', 2) +
        " zł");
}

void ExpenseReportWindow::generateReport()
{
    if (!m_data) {
        QMessageBox::critical(
            this,
            "Błąd",
            "Brak dostępu do danych.");

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

    QVector<CategoryExpenseReport> report;
    double totalExpenses = 0.0;
    QString error;

    if (!m_data->getExpenseBreakdown(
            start,
            end,
            report,
            totalExpenses,
            error)) {

        QMessageBox::critical(
            this,
            "Błąd raportu",
            "Nie udało się wygenerować raportu:\n" + error);

        return;
    }

    ui->tableWidgetReport->setRowCount(0);

    QLocale locale(QLocale::Polish, QLocale::Poland);

    for (const CategoryExpenseReport &category : report) {

        // Wiersz kategorii
        int row = ui->tableWidgetReport->rowCount();

        ui->tableWidgetReport->insertRow(row);

        auto *categoryName = new QTableWidgetItem(category.Name);
        auto *categoryAmount = new QTableWidgetItem(
            locale.toString(category.Amount, 'f', 2) + " zł");

        auto *categoryPercent = new QTableWidgetItem(
            locale.toString(category.Percent, 'f', 2) + "%");

        QFont boldFont;
        boldFont.setBold(true);

        categoryName->setFont(boldFont);
        categoryAmount->setFont(boldFont);
        categoryPercent->setFont(boldFont);

        ui->tableWidgetReport->setItem(row, 0, categoryName);
        ui->tableWidgetReport->setItem(row, 1, categoryAmount);
        ui->tableWidgetReport->setItem(row, 2, categoryPercent);

        categoryAmount->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        categoryPercent->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        // Wiersze podkategorii
        for (const SubcategoryExpenseReport &subcategory :
             category.Subcategories) {

            row = ui->tableWidgetReport->rowCount();

            ui->tableWidgetReport->insertRow(row);

            auto *subcategoryName = new QTableWidgetItem(
                "    " + subcategory.Name);

            auto *subcategoryAmount = new QTableWidgetItem(
                locale.toString(subcategory.Amount, 'f', 2) + " zł");

            auto *subcategoryPercent = new QTableWidgetItem(
                locale.toString(subcategory.Percent, 'f', 2) + "%");

            ui->tableWidgetReport->setItem(row, 0, subcategoryName);
            ui->tableWidgetReport->setItem(row, 1, subcategoryAmount);
            ui->tableWidgetReport->setItem(row, 2, subcategoryPercent);

            subcategoryAmount->setTextAlignment(
                Qt::AlignRight | Qt::AlignVCenter);

            subcategoryPercent->setTextAlignment(
                Qt::AlignRight | Qt::AlignVCenter);
        }
    }

    updateTotal(totalExpenses);

    ui->tableWidgetReport->resizeRowsToContents();
}

void ExpenseReportWindow::on_pushButton_back_clicked()
{
    emit backToAnalytics();
}

