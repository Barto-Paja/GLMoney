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
    QString SubcategoryName;

    Payee() = default;
    ~Payee() = default;

    Payee(const int & id, const QString & name, const int & suggested_category_id, const QString & subcategory_name)
        : ID { id }, Name { name }, SuggestedSubCategoryID { suggested_category_id }, SubcategoryName{ subcategory_name }
    {

    }
};

/*!
 * \brief The Category struct
 *
 */
struct Category {
    int ID;
    QString Name;

    Category() = default;
    ~Category() = default;

    Category(const int & id, const QString & name)
        : ID { id }, Name { name }
    {

    }
};

/*!
 * \brief The SubCategory struct
 */
struct SubCategory {
    int ID;
    QString Name;
    int RootID;

    SubCategory() = default;
    ~SubCategory() = default;

    SubCategory(const int & id, const QString & name, const int & root_id)
        : ID { id }, Name { name }, RootID { root_id }
    {

    }
};

/*!
 * \brief The Account class
 *
 * \note W tym kontekście konto, to jest źródło, z którego ściągane są środki
 */
struct Account {
    int ID;
    QString Name;

    Account() = default;
    ~Account() = default;

    Account(const int & id, const QString & name)
        : ID { id }, Name { name }
    {

    }
};

/*!
 * \brief The transactionsResume struct
 */

enum class balance{
    expanse = 0,
    income
};

struct TransactionsResume
{
    balance Type { balance::expanse };
    double Amount {0.0};
    int Year;
    int Month;

    TransactionsResume() = default;
    ~TransactionsResume() = default;

    TransactionsResume(balance type, double amount, int year, int month)
        : Type{type}, Amount{amount}, Year{year}, Month{month}
    {}
};

struct TransactionHistory
{
    int ID {0};
    double Amount {0.00};
    QDate Date;
    QString Payee;
    QString Category;
    QString Subcategory;
    QString Member;
    QString Note;
    QString Account;
    int AccountID;

    TransactionHistory() = default;
    ~TransactionHistory() = default;

    TransactionHistory(int id, double amount, QDate date, QString payee,
                       QString category, QString subcategory, QString member, QString note, QString account, int account_id)
        : ID            { id },
          Amount        { amount },
          Date          { date },
          Payee         { payee },
          Category      { category },
          Subcategory   { subcategory },
          Member        { member },
          Note          { note },
          Account       { account },
          AccountID     { account_id }
    {}
};

struct HomeBudgetCalculation
{
    int ID { 0 };
    double DeclaredIncome {0.00};
    QDate Date;
    bool IsValid { false };

    HomeBudgetCalculation() = default;
    ~HomeBudgetCalculation() = default;

    HomeBudgetCalculation(int id, double income, const QDate & date) :
        ID { id },
        DeclaredIncome { income },
        Date { date }
    {}
};

struct FixedExpanseCalculation
{
    int ID { 0 };
    int HomeBudgetCalculationID { 0 };
    int CategoryID { 0 };
    double Amount {0.00};
    QString Category;

    FixedExpanseCalculation() = default;
    ~FixedExpanseCalculation() = default;

    FixedExpanseCalculation(int id, int homebudgetcacl_id, int category_id,
                            double amount, const QString & category_name) :
        ID { id },
        HomeBudgetCalculationID { homebudgetcacl_id },
        CategoryID { category_id },
        Amount { amount },
        Category { category_name }
    {}

};

struct OneOffExapanseCalculation
{
    int ID { 0 };
    int HomeBudgetCalculationID { 0 };
    double Amount {0.00};
    QString Name;
    int CategoryID {0};
    QString CategoryName;

    OneOffExapanseCalculation() = default;
    ~OneOffExapanseCalculation() = default;

    OneOffExapanseCalculation(int id, int home_budget_cacl_id, double amount,
                              const QString & name, const int & category) :
        ID { id },
        HomeBudgetCalculationID { home_budget_cacl_id },
        Amount { amount },
        Name { name },
        CategoryID { category }
    {}

    OneOffExapanseCalculation(int id, int home_budget_cacl_id, double amount,
                              const QString & name, const int & category, const QString & name_cat) :
        ID { id },
        HomeBudgetCalculationID { home_budget_cacl_id },
        Amount { amount },
        Name { name },
        CategoryID { category },
        CategoryName { name_cat }
    {}
};


struct TransferBetweenAccounts
{
    long SourceID;
    long SourcePayeeMirror;
    long SourceSubcategory;

    long TargetID;
    long TargetPayeeMirror;
    long TargetSubcategory;

    long MemberID;

    double Amount;

    QDate Date;

    TransferBetweenAccounts() = default;
    ~TransferBetweenAccounts() = default;
};

struct CategoryResume
{
    QString CategoryName;
    double PlannedExpense;
    double CalculatedExpense;

    CategoryResume() = default;
    ~CategoryResume() = default;
};

#endif // DATASTRUCTS_H
