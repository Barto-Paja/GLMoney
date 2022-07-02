#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include <QString>
#include <QDate>

/*!
 * \brief The Member struct
 *
 * \note To jest traktowany jako członek rodziny, do niego przypisywane są koszta.
 */
struct Member {
    int ID;
    QString Name;

    Member() = default;
    ~Member() = default;

    Member(const int & id, const QString & name) :
        ID{ id }, Name{ name }
    {

    }
};

/*!
 * \brief The Payee struct
 *
 * \note To jest traktowany jako płatnik, kontrahent - do niego przypisywane są traksakcje.
 */
struct Payee {
    int ID;
    QString Name;
    int SuggestedSubCategoryID;

    Payee() = default;
    ~Payee() = default;

    Payee(const int & id, const QString & name, const int & suggested_category_id)
        : ID { id }, Name { name }, SuggestedSubCategoryID { suggested_category_id }
    {

    }
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

struct TransactionHistory
{
    int id {0};
    double amount {0.00};
    QDate date;
    QString payee;
    QString category;
    QString subcategory;
    QString member;
    QString note;
    QString account;
    int accountID;

    TransactionHistory(int id_v, double amount_v, QDate date_v, QString payee_v,
                       QString category_v, QString subcategory_v, QString member_v, QString note_v, QString account_v, int account_id)
        : id            { id_v },
          amount        { amount_v },
          date          { date_v },
          payee         { payee_v },
          category      { category_v },
          subcategory   { subcategory_v },
          member        { member_v },
          note          { note_v},
          account       { account_v},
          accountID     { account_id}
    {}
};

struct HomeBudgetCalculation
{
    int id { 0 };
    double declaredIncome {0.00};
    QDate date;
    bool isValid { false };

    HomeBudgetCalculation(){}
    HomeBudgetCalculation(int id_v, double income_v, const QDate & date_v) :
        id { id_v },
        declaredIncome { income_v },
        date { date_v }
    {}
};

struct FixedExpanseCalculation
{
    int id { 0 };
    int homeBudgetCalculationID { 0 };
    int categoryID { 0 };
    double amount {0.00};
    QString category;

    FixedExpanseCalculation(int id_v, int homebudgetcacl_id_v, int categoryID_v,
                            double amount_v, const QString & categoryName) :
        id { id_v },
        homeBudgetCalculationID { homebudgetcacl_id_v },
        categoryID { categoryID_v },
        amount { amount_v },
        category { categoryName }
    {}

};

struct OneOffExapanseCalculation
{
    int id { 0 };
    int homeBudgetCalculationID { 0 };
    double amount {0.00};
    QString name;
    int categoryID {0};
    QString category_name;

    OneOffExapanseCalculation(int id_v, int homebudgetcacl_id_v, double amount_v,
                              const QString & name_v, const int & category) :
        id { id_v },
        homeBudgetCalculationID { homebudgetcacl_id_v },
        amount { amount_v },
        name { name_v },
        categoryID { category }
    {}

    OneOffExapanseCalculation(int id_v, int homebudgetcacl_id_v, double amount_v,
                              const QString & name_v, const int & category, const QString & name_cat) :
        id { id_v },
        homeBudgetCalculationID { homebudgetcacl_id_v },
        amount { amount_v },
        name { name_v },
        categoryID { category },
        category_name { name_cat }
    {}
};


struct TransferBetweenAccounts
{
    long sourceID;
    long sourcePayeeMirror;
    long sourceSubcategory;

    long targetID;
    long targetPayeeMirror;
    long targetSubcategory;

    long memberID;

    double amount;

    QDate date;

    TransferBetweenAccounts(){}
};

struct CategoryResume
{
    QString categoryName;
    double plannedExpense;
    double calculatedExpense;

    CategoryResume(){}
};

#endif // DATASTRUCTS_H
