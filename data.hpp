#ifndef DATA_HPP
#define DATA_HPP

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QDate>
#include <QMap>
#include <QProgressBar>
#include <QLabel>

#include <QDebug>

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
    QString guiNote;
};

class Data : public QObject
{
    Q_OBJECT

public:
    explicit Data(QObject *parent = nullptr);

    /*!
     * \brief Zwraca listę Centrum Kosztów
     * \return
     */
    QVector<Member> members() const;
    /*!
     * \brief Zwraca listę
     * \return
     */
    QVector<Payee> payees() const;
    QVector<Category> categories() const;
    QVector<SubCategory> subCategories(int index) const;
    QMap<int,QVector<SubCategory>> subCategories() const;
    QVector<Transaction> transactions() const;
    QVector<Account> accounts() const;

    bool addTransaction(const newTransaction &transaction, QString & error);
    bool AddTransactions(const QVector<newTransaction> &transactions, QString & error, QProgressBar * pg, QLabel * lb);
    bool addPayee(const QString & name, const QString description, QString & error);
    bool addCategory(const QString & name, QString & error);
    bool addSubcategory(const QString & name, int categoryID, QString & error);
    bool addMember(const QString & name, QString & error);
    bool addAccount(const QString & name, const QString & description, QString & error);

    bool GetTransactionsResume(int start_year, int end_year, QVector<TransactionsResume> & transactions_history_expanse, QVector<TransactionsResume> &transactions_history_income, QString &error);
    bool getTransactionsHistory(QDate start, QDate end, QVector<TransactionHistory> &transactions_history, QString & error);

    void reloadPayee();
    void reloadCategory();
    void reloadMember();
    void reloadAccount();

    bool commitHomeBudgetCalculation(HomeBudgetCalculation & home_calc, QVector<FixedExpanseCalculation> & fixed, QVector<OneOffExapanseCalculation> & oneoff, QString & error );
    bool checkIsHomeBudgetCalculationExist(const QDate & date, HomeBudgetCalculation & home_calc, QString & error);
    bool loadHomeBudgetExpanses(const HomeBudgetCalculation & hbc, QVector<FixedExpanseCalculation> & fixed, QVector<OneOffExapanseCalculation> & oneoff, QString & error);

    bool makeTransactionBetweenAccounts(const TransferBetweenAccounts & transferData, QString &error);
    bool getResume(const QDate & date, QVector<CategoryResume> & resumes, QString & error);

    Payee GetPayee(const int & id) const;
    SubCategory GetSubCategory(const int & id) const;

    bool UpdatePayee(const Payee & payee);


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
