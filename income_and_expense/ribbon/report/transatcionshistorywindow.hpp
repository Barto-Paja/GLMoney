#ifndef TRANSATCIONSHISTORYWINDOW_HPP
#define TRANSATCIONSHISTORYWINDOW_HPP

#include <QWidget>
#include "data.hpp"

namespace Ui {
class TransatcionsHistoryWindow;
}

class TransatcionsHistoryWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TransatcionsHistoryWindow(Data * data, QWidget *parent = nullptr);
    ~TransatcionsHistoryWindow();

private slots:
    void on_dateEdit_from_userDateChanged(const QDate &date);

private:
    Ui::TransatcionsHistoryWindow *ui;

    Data * m_data {nullptr};
    QVector<TransactionHistory> m_transactions;

    bool fillTable();
};

#endif // TRANSATCIONSHISTORYWINDOW_HPP
