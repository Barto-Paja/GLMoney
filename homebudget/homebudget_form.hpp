#ifndef HOMEBUDGET_FORM_HPP
#define HOMEBUDGET_FORM_HPP

#include <QWidget>
#include "../data.hpp"

#include <QMessageBox>

namespace Ui {
class HomeBudget_Form;
}

class HomeBudget_Form : public QWidget
{
    Q_OBJECT

public:
    explicit HomeBudget_Form(Data * data , QWidget *parent = nullptr);
    ~HomeBudget_Form();

signals:

void back();



private slots:
    void on_pushButton_remove_fixedExpense_clicked();
    void on_pushButton_insert_fixedExpense_clicked();
    void on_pushButton_remove_Expense_clicked();
    void on_pushButton_insert_Expense_clicked();
    void on_pushButton_canel_clicked();
    void on_pushButton_commit_clicked();
    void on_lineEdit_income_textChanged(const QString &arg1);
    void on_dateEdit_dateChanged(const QDate &date);

    void on_pushButton_back_clicked();

    void on_pushButton_Resume_clicked();

private:
    Ui::HomeBudget_Form *ui;

    Data * m_data { nullptr };

    HomeBudgetCalculation m_hbc;
    QVector<FixedExpanseCalculation> m_fixedExpanses;
    QVector<OneOffExapanseCalculation> m_oneoffExpanses;

    QString m_lastError;

    void calulate();

};

#endif // HOMEBUDGET_FORM_HPP
