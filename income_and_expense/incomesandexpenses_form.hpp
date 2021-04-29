#ifndef INCOMESANDEXPENSES_FORM_HPP
#define INCOMESANDEXPENSES_FORM_HPP

#include <QWidget>
#include <QLocale>
#include <QMessageBox>
#include "data.hpp"

#include "ribbon/core.h"
#include "ribbon/export.h"
#include "ribbon/report.h"
#include "ribbon/utils.h"

namespace Ui {
class IncomesAndExpenses_Form;
}

class IncomesAndExpenses_Form : public QWidget
{
    Q_OBJECT

public:
    explicit IncomesAndExpenses_Form(Data * data = nullptr, QWidget *parent = nullptr);
    ~IncomesAndExpenses_Form();

signals:

    void back();

private slots:
    void on_comboBox_category_currentIndexChanged(int index);
    void on_calendarWidget_clicked(const QDate &date);
    void on_checkBox_isIncome_stateChanged(int arg1);
    void on_pushButton_Commit_clicked();
    void on_pushButton_Back_clicked();
    void on_pushButton_dateSelect_clicked();

    // Adding new elements

    void on_toolButton_addCategory_clicked();
    void on_toolButton_addSubcategory_clicked();
    void on_toolButton_addAccount_clicked();
    void on_toolButton_addPayee_clicked();
    void on_toolButton_addMember_clicked();

    void on_toolButton_makeTransactionsBetweenAccounts_clicked();

    // Reports
    void on_toolButton_reportMonth_clicked();

    // Exports
    void on_toolButton_exportToCSV_clicked();

private:
    Ui::IncomesAndExpenses_Form *ui;

    Data * m_data { nullptr };

    void fillComboboxes();
    void reloadSubcategories(int index);
    void reloadPayees();
    void reloadCategories();
    void reloadAccounts();
    void reloadMembers();

};

#endif // INCOMESANDEXPENSES_FORM_HPP
