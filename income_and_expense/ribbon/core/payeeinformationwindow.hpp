#ifndef PAYEEINFORMATIONWINDOW_HPP
#define PAYEEINFORMATIONWINDOW_HPP

#include <QDialog>
#include "data.hpp"

namespace Ui {
class PayeeInformationWindow;
}

class PayeeInformationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PayeeInformationWindow(const int & payee_id ,Data * data, QWidget *parent = nullptr);
    ~PayeeInformationWindow();

private slots:
    void on_comboBox_category_currentIndexChanged(int index);

    void on_pushButton_save_clicked();

private:
    Ui::PayeeInformationWindow *ui;
    Data * m_data { nullptr };
    int m_payeeID{-1};
    Payee data;

    bool LoadCategories();
    void ReloadSubcategories(int index);
    bool LoadPayee();
};

#endif // PAYEEINFORMATIONWINDOW_HPP
