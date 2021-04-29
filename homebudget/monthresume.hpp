#ifndef MONTHRESUME_HPP
#define MONTHRESUME_HPP

#include <QDialog>
#include <QDate>
#include "../data.hpp"

namespace Ui {
class MonthResume;
}

class MonthResume : public QDialog
{
    Q_OBJECT

public:
    explicit MonthResume(Data * data, const QDate & date ,QWidget *parent = nullptr);
    ~MonthResume();

private:
    Ui::MonthResume *ui;

    Data * m_data { nullptr };
    QDate m_date;
};

#endif // MONTHRESUME_HPP
