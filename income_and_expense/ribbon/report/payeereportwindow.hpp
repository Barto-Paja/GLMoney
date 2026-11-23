#ifndef PAYEEREPORTWINDOW_HPP
#define PAYEEREPORTWINDOW_HPP

#include <QWidget>

#include "data.hpp"

namespace Ui {
class PayeeReportWindow;
}

class PayeeReportWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PayeeReportWindow(Data *data, QWidget *parent = nullptr);
    ~PayeeReportWindow();

signals:
    void backToAnalytics();

private slots:
    void generateReport();

    void on_pushButton_back_clicked();

private:
    Ui::PayeeReportWindow *ui;
    Data *m_data;

    void setupTable();
    void loadPayees();
    void updateSummary(double income, double expenses, double balance);
};

#endif // PAYEEREPORTWINDOW_HPP
