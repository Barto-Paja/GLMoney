#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include <QString>

/*!
 * \brief The Member struct
 */

struct Member {
    int id;
    QString name;
};

struct Payee {
    int id;
    QString name;
};

struct Category {
    int id;
    QString name;
};

struct SubCategory {
    int id;
    QString name;
    int rootID;
};

struct Account {
    int id;
    QString name;
};

/*!
 * \brief The transactionsResume struct
 */

enum class balance{
    expanse = 0,
    income
};

struct transactionsResume
{
    balance type { balance::expanse };
    double amount {0.0};
    int year;
    int month;

    transactionsResume(balance type_v, double amount_v, int y, int m)
        : type(type_v), amount(amount_v), year(y), month(m)
    {}
};

#endif // DATASTRUCTS_H
