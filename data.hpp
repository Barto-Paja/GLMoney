#ifndef DATA_HPP
#define DATA_HPP

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QDate>
#include <QMap>

#include "DataStructs.h"

struct Transaction {

    int id;
    QDate date;

    Member member;
    Payee payee;
    double amount {0.00};
    Category category;
    SubCategory subCategory;

    int accountId {0};
    int multiplicand {-1};
    QString note;

};

struct newTransaction {
    QDate date;
    int payeeID {0};
    double amount {0.00};
    int categoryID {0};
    int memberID {0};
    int accountID{0};
    QString note;
    int multiplicand {-1};
};

class Data : public QObject
{
    Q_OBJECT

public:
    explicit Data(QObject *parent = nullptr);

    QVector<Member> members() const;
    QVector<Payee> payees() const;
    QVector<Category> categories() const;
    QVector<SubCategory> subCategories(int index) const;
    QMap<int,QVector<SubCategory>> subCategories() const;
    QVector<Transaction> transactions() const;
    QVector<Account> accounts() const;

    bool addTransaction(const newTransaction &transaction);

signals:

private:

    QSqlDatabase m_db;

    QVector<Member> m_members;
    QVector<Payee> m_payees;
    QVector<Category> m_categories;
    QMap<int,QVector<SubCategory>> m_subCategories;
    QVector<Account> m_accounts;

    QVector<Transaction> m_transactions;

    bool loadData();
    bool loadDictionaries();
    bool loadMembers();
    bool loadPayees();
    bool loadCategories();
    bool loadAccounts();
    bool loadTransactions();

};

#endif // DATA_HPP
