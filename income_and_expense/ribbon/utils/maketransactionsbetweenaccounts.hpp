#ifndef MAKETRANSACTIONSBETWEENACCOUNTS_HPP
#define MAKETRANSACTIONSBETWEENACCOUNTS_HPP

#include <QDialog>
#include "data.hpp"
#include <QComboBox>

#include <QMessageBox>

namespace Ui {
class MakeTransactionsBetweenAccounts;
}

class MakeTransactionsBetweenAccounts : public QDialog
{
    Q_OBJECT

public:
    explicit MakeTransactionsBetweenAccounts(Data * data, QWidget *parent = nullptr);
    ~MakeTransactionsBetweenAccounts();

private slots:
    void on_comboBox_target_category_currentIndexChanged(int index);

    void on_comboBox_source_category_currentIndexChanged(int index);

    void on_calendarWidget_clicked(const QDate &date);

    void on_pushButton_Commit_clicked();

    void on_pushButton_Canel_clicked();

private:

    enum class SRC
    {
        SOURCE = 0,
        TARGET
    };

    void reloadSubcategories(SRC src, int index);

    Ui::MakeTransactionsBetweenAccounts *ui;
    Data * m_data {nullptr};
};

#endif // MAKETRANSACTIONSBETWEENACCOUNTS_HPP
