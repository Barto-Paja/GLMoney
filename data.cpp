#include "data.hpp"
#include <QVariant>
#include <QDebug>

Data::Data(QObject *parent) : QObject(parent)
{
    loadData();
}

QVector<Member> Data::members() const
{
    return m_members;
}

QVector<Payee> Data::payees() const
{
    return m_payees;
}

QVector<Category> Data::categories() const
{
    return m_categories;
}

QVector<SubCategory> Data::subCategories(int index) const
{
    return m_subCategories.find(index).value();
}

QMap<int, QVector<SubCategory> > Data::subCategories() const
{
    return m_subCategories;
}

QVector<Transaction> Data::transactions() const
{
    return m_transactions;
}

QVector<Account> Data::accounts() const
{
    return m_accounts;
}

bool Data::AddTransactions(const QVector<newTransaction> &transactions, QString &error, QProgressBar *pg, QLabel * lb)
{
    QString msg_error = "Transakcja nr:  nie powiodła się:\n";
    pg->setMaximum(transactions.size());

    int index = 1;
    for(const auto & transaction : transactions)
    {
        pg->setValue(index);
        lb->setText("Dodawanie transakcji nr "+QString::number(index)+"\n" + transaction.guiNote);
        QString error_t;
        if(!addTransaction(transaction,error_t))
        {
            msg_error += error_t;
            error = msg_error;
            qDebug() << msg_error;
            return false;
        }
        index++;

    }

    return true;
}

bool Data::addTransaction(const newTransaction &transaction, QString &error)
{
    QSqlQuery query {m_db};

    m_db.transaction();

    query.prepare("INSERT INTO [Transaction] "
                  "([when], payee_id, amount, category_id, member_id, note, acount_id, multiplicand ) "
                  "VALUES "
                  "( :when, :payee_id, :amount, :categoryid, :memberid, :note, :accountid, :multiplicand )");

    query.bindValue(":when",transaction.date.toString("yyyy-MM-dd"));
    query.bindValue(":payee_id",transaction.payeeID);
    query.bindValue(":amount", transaction.amount);
    query.bindValue(":categoryid",transaction.categoryID);
    query.bindValue(":memberid",transaction.memberID);
    query.bindValue(":note",transaction.note);
    query.bindValue(":accountid",transaction.accountID);
    query.bindValue(":multiplicand",transaction.multiplicand);

    if(query.exec())
    {
        m_db.commit();
        return true;
    }
    else
    {
        error = query.lastError().text();
        m_db.rollback();
        return false;
    }
}

bool Data::addPayee(const QString &name, const QString description, QString &error)
{
    QSqlQuery query {m_db};

    m_db.transaction();

    query.prepare("INSERT INTO Payee ( name, description ) VALUES ( :name, :description )");
    query.bindValue(":name", name);
    query.bindValue(":description", description);

    if(query.exec())
    {
        m_db.commit();
        return true;
    }
    else
    {
        error = query.lastError().text();
        m_db.rollback();
        return false;
    }
}

bool Data::addCategory(const QString &name, QString &error)
{
    QSqlQuery query {m_db};
    QString query_text {
      "INSERT INTO Category ( name ) VALUES ( :name ) "
    };

     m_db.transaction();

     query.prepare(query_text);
     query.bindValue(":name",name);

     if(query.exec())
     {
         m_db.commit();
         return true;
     }
     else
     {
         error = query.lastError().text();
         m_db.rollback();
         return false;
     }
}

bool Data::addSubcategory(const QString &name, int categoryID, QString &error)
{
    QSqlQuery query { m_db };
    QString query_text {
        "INSERT INTO Subcategory ( name, category_id ) VALUES ( :name, :catID ) "
    };

    m_db.transaction();

    query.prepare(query_text);
    query.bindValue(":name",name);
    query.bindValue(":catID", categoryID);

    if(query.exec())
    {
        m_db.commit();
        return true;
    }
    else
    {
        error = query.lastError().text();
        m_db.rollback();
        return false;
    }

}

bool Data::addMember(const QString &name, QString &error)
{
    QSqlQuery query { m_db};
    QString query_text {
        "INSERT INTO Member ( name ) VALUES ( :name ) "
    };

    m_db.transaction();

    query.prepare(query_text);
    query.bindValue(":name",name);

    if(query.exec())
    {
        m_db.commit();
        return true;
    }
    else
    {
        error = query.lastError().text();
        m_db.rollback();
        return false;
    }

}

bool Data::addAccount(const QString &name, const QString &description, QString &error)
{
    QSqlQuery query {m_db};
    QString query_text {
        "INSERT INTO Account ( name, description ) VALUES ( :name, :description ) "
    };

    m_db.transaction();

    query.prepare(query_text);
    query.bindValue(":name",name);
    query.bindValue(":description",description);

    if(query.exec())
    {
        m_db.commit();
        return true;
    }
    else
    {
        error = query.lastError().text();
        m_db.rollback();
        return false;
    }
}

bool Data::getTransactionsResume(int start_year, int end_year, QVector<transactionsResume> &transactions_history_expanse, QVector<transactionsResume> &transactions_history_income, QString & error)
{
    QSqlQuery query {m_db};
    QString query_text {
        "select SUM([Transaction].amount) as 'Balance', "
        "strftime('%Y', [when]) as 'year', "
        "strftime('%m', [when]) as 'month' "
        "from [Transaction] "
        "where multiplicand > 0 AND ([when] between '"+QString::number(start_year)+"-01-01' AND '"+QString::number(end_year)+"-12-31') "
        "group by strftime('%m-%Y',  [when]) order by year,month ASC "
    };

    query.prepare(query_text);

    qDebug() << query.lastQuery();

    if(query.exec())
    {
        while(query.next())
        {
            transactions_history_expanse.append(transactionsResume{
                balance::expanse,
                query.value("Balance").toDouble(),
                query.value("year").toInt(),
                query.value("month").toInt()
            });
        }
    }
    else
    {
        error = query.lastError().text();
        return false;
    }

    query_text = (
        "select SUM([Transaction].amount) as 'Balance', "
        "strftime('%Y', [when]) as 'year', "
        "strftime('%m', [when]) as 'month' "
        "from [Transaction] "
        "where multiplicand < 0 AND ([when] between '"+QString::number(start_year)+"-01-01' AND '"+QString::number(end_year)+"-12-31') "
        "group by strftime('%m-%Y',  [when]) order by year,month ASC "
        );

    query.prepare(query_text);

    qDebug() << query.lastQuery();

    if(query.exec())
    {
        while(query.next())
        {
            transactions_history_income.append(transactionsResume{
                balance::income,
                query.value("Balance").toDouble(),
                query.value("year").toInt(),
                query.value("month").toInt()
            });
        }

        return true;
    }
    else
    {
        error = query.lastError().text();
        return false;
    }

}

bool Data::getTransactionsHistory(QDate start, QDate end, QVector<TransactionHistory> &transactions_history, QString &error)
{
    QSqlQuery query {m_db};

    QString query_text {
        "select count(*) as counter from [Transaction]"
    };

    query.prepare(query_text);
    if(query.exec() && query.next())
    {
        if( query.value("counter").toInt() == transactions_history.size())
        {
            return true;
        }
        else
        {
            transactions_history.clear();
        }
    }


    query_text = "SELECT transaction_id, [when] as date, Payee.name as payee, "
        "( amount * multiplicand ) as amount, "
        "Category.name  as category, Subcategory.name as subcategory, "
        "Member.name as member, [Transaction].acount_id as account_id, "
        "note, Account.name as account  FROM [Transaction] "
        "INNER JOIN Payee ON Payee.payee_id = [Transaction].payee_id "
        "INNER JOIN Member ON Member.member_id = [Transaction].member_id "
        "INNER JOIN Subcategory ON Subcategory.subcategory_id = [Transaction].category_id "
        "INNER JOIN Category ON Category.category_id = Subcategory.category_id "
        "INNER JOIN Account ON  Account.account_id = [Transaction].acount_id "
        "WHERE ([when] BETWEEN '"+start.toString("yyyy-MM-dd")+"' AND '"+end.toString("yyyy-MM-dd")+"') "
        "ORDER BY [when] DESC";

    query.prepare(query_text);
    query.bindValue(":start_date",start.toString("yyyy-MM-dd"));
    query.bindValue(":end_date",end.toString("yyyy-MM-dd"));

    qDebug() << query.lastQuery() << start.toString("yyyy-MM-dd") << end.toString("yyyy-MM-dd");

    if(query.exec())
    {
        while(query.next())
        {
            transactions_history.append(TransactionHistory{
                query.value("transaction_id").toInt(),
                query.value("amount").toDouble(),
                query.value("date").toDate(),
                query.value("payee").toString(),
                query.value("category").toString(),
                query.value("subcategory").toString(),
                query.value("member").toString(),
                query.value("note").toString(),
                query.value("account").toString(),
                query.value("account_id").toInt()
            });
        }

        return true;
    }
    else
    {
        error = query.lastError().text();
        qDebug() << error;
        return false;
    }

}

void Data::reloadPayee()
{
    m_payees.clear();

    loadPayees();
}

void Data::reloadCategory()
{
    m_categories.clear();
    m_subCategories.clear();

    loadCategories();
}

void Data::reloadMember()
{
    m_members.clear();

    loadMembers();
}

void Data::reloadAccount()
{
    m_accounts.clear();

    loadAccounts();
}

bool Data::commitHomeBudgetCalculation(HomeBudgetCalculation &home_calc, QVector<FixedExpanseCalculation> &fixed, QVector<OneOffExapanseCalculation> &oneoff, QString &error)
{
    QSqlQuery query {m_db};
    m_db.transaction();

    QString queryText;
    if(!home_calc.isValid)
    {
        queryText =
            "INSERT INTO Home_budget_calculation ( period, declared_income ) "
            " VALUES ( '" +
            home_calc.date.toString("yyyy-MM-dd") + "', "
            + QString::number(home_calc.declaredIncome) + ") "
        ;

        query.prepare(queryText);
        if(!query.exec())
        {
            error = query.lastError().text() + "\n" + query.lastQuery();
            m_db.rollback();
            return false;
        }

        checkIsHomeBudgetCalculationExist(home_calc.date,home_calc,error);

    }


        for(const auto &i : fixed)
        {
            queryText = "INSERT INTO Fixed_expense ( category_id, amount, home_budget_calculation, category_id ) "
                        "VALUES "
                        "( :category_id, :amount, :home_budget_calculation, :category_id )";

            query.prepare(queryText);
            query.bindValue(":category_id",i.categoryID);
            query.bindValue(":amount",i.amount);
            query.bindValue(":home_budget_calculation",home_calc.id);
            query.bindValue(":category_id",i.categoryID);

            if(query.exec())
            {

            }
            else
            {
                error = query.lastError().text() + "\n" + query.lastQuery();
                m_db.rollback();
                return false;
            }

        }

    for(const auto &i : oneoff)
    {
        queryText = "INSERT INTO One_off_expense ( name, amount, home_budget_id, category_id ) "
                    "VALUES "
                    "( :name, :amount, :home_budget_id )";

        query.prepare(queryText);
        query.bindValue(":name",i.name);
        query.bindValue(":amount",i.amount);
        query.bindValue(":home_budget_id",home_calc.id);
        query.bindValue(":category_id",i.categoryID);

        if(query.exec())
        {

        }
        else
        {
            error = query.lastError().text() + "\n" + query.lastQuery();
            m_db.rollback();
            return false;
        }
    }

    m_db.commit();
    return true;
}

bool Data::checkIsHomeBudgetCalculationExist(const QDate &date, HomeBudgetCalculation &home_calc, QString &error)
{
    QSqlQuery query {m_db};

    QString queryText = "SELECT home_budget_calculation_id, period, declared_income "
                        "FROM Home_budget_calculation "
                        "WHERE (period BETWEEN '"+date.toString("yyyy-MM-01")+"' AND '"+date.addMonths(1).toString("yyyy-MM-dd")+"') "
                        "LIMIT 1";
    query.prepare(queryText);

    if(query.exec())
    {
        if(query.next())
        {
            home_calc.id = query.value("home_budget_calculation_id").toInt();
            home_calc.date = query.value("period").toDate();
            home_calc.declaredIncome = query.value("declared_income").toDouble();
            home_calc.isValid = true;
            return true;
        }
        else
        {
            home_calc = HomeBudgetCalculation();
        }
    }
    else
    {
        home_calc = HomeBudgetCalculation();
        error = query.lastError().text();
    }

    return false;
}

bool Data::loadHomeBudgetExpanses(const HomeBudgetCalculation & hbc,QVector<FixedExpanseCalculation> &fixed, QVector<OneOffExapanseCalculation> &oneoff, QString &error)
{
    QSqlQuery query {m_db};
    QString queryText { "SELECT fe.fixed_expense_id, fe.category_id, fe.amount, fe.home_budget_calculation, c.name "
                        "FROM Fixed_expense fe "
                        "inner join category c on c.category_id = fe.category_id "
                        "WHERE home_budget_calculation = :home_budget_calculation" };
    query.prepare(queryText);
    query.bindValue(":home_budget_calculation",hbc.id);

    if(query.exec())
    {
        while(query.next())
        {
            fixed.append(FixedExpanseCalculation{
                             query.value("fixed_expense_id").toInt(),
                             query.value("home_budget_calculation").toInt(),
                             query.value("category_id").toInt(),
                             query.value("amount").toDouble(),
                             query.value("name").toString()
                         });
        }
    }
    else
    {
        error = query.lastError().text();
        return false;
    }

    queryText = "SELECT ofe.one_of_expense_id, ofe.name, ofe.amount, ofe.home_budget_id, ofe.category_id, c.name as name_c FROM One_off_expense ofe "
                "INNER JOIN Category c on c.category_id = ofe.category_id AND ofe.home_budget_id = :home_budget_id";

    query.prepare(queryText);
    query.bindValue(":home_budget_id",hbc.id);

    if(query.exec())
    {
        while(query.next())
        {
            oneoff.append(OneOffExapanseCalculation{
                             query.value("one_of_expense_id").toInt(),
                             query.value("home_budget_id").toInt(),
                             query.value("amount").toDouble(),
                             query.value("name").toString(),
                             query.value("category_id").toInt(),
                             query.value("name_c").toString()
                          });
        }
    }
    else
    {
        error = query.lastError().text();
        return false;
    }

    return true;
}

bool Data::makeTransactionBetweenAccounts(const TransferBetweenAccounts & transferData, QString &error)
{
    if(!addTransaction(newTransaction{
                       transferData.date,
                       transferData.sourcePayeeMirror,
                       transferData.amount,
                       transferData.targetSubcategory,
                       transferData.memberID,
                       transferData.sourceID,
                       "Transfer środków pomiędzy kontami",
                       -1
                   }, error))
    {
        return false;
    }

    return addTransaction(newTransaction{
                              transferData.date,
                              transferData.targetPayeeMirror,
                              transferData.amount,
                              transferData.sourceSubcategory,
                              transferData.memberID,
                              transferData.targetID,
                              "Transfer środków pomiędzy kontami",
                              1
                          }, error);
}

bool Data::getResume(const QDate &date, QVector<CategoryResume> &resumes, QString &error)
{
    QString query_text {
        "SELECT category_id, name FROM Category"
    };

    QSqlQuery query     { m_db };
    QSqlQuery query2    { m_db };

    query.prepare(query_text);

    if(query.exec())
    {
        while(query.next())
        {
            CategoryResume resume;
            int categoryID { 0 };

            resume.categoryName = query.value("name").toString();
            categoryID = query.value("category_id").toInt();

            query_text =
                    "select sum(t.amount) as amount from [Transaction] t "
                    "inner join subcategory sc on t.category_id = sc.subcategory_id "
                    "left outer join category c on sc.category_id = c.category_id "
                    //"where t.multiplicand = -1 "
                    "where (t.[when] BETWEEN '"+date.toString("yyyy-MM-01")+"' AND '"+date.addMonths(1).toString("yyyy-MM-dd")+"') "
                    "AND c.category_id = " + QString::number(categoryID);

            qDebug() << query_text;

            query2.prepare(query_text);

            if(query2.exec())
            {
                if(query2.next())
                {
                    resume.calculatedExpense = query2.value("amount").toDouble();
                }
            }
            else
            {
                error = query2.lastError().text();
            }

            query_text = "select home_budget_calculation_id from home_budget_calculation hbd where hbd.period BETWEEN "
            "'"+date.toString("yyyy-MM-01")+"' AND '"+date.addMonths(1).toString("yyyy-MM-dd")+"'";

            qDebug() << query_text;

            query2.prepare(query_text);

            int homeBudgetID { 0 };

            if(query2.exec())
            {
                if(query2.next())
                {
                    homeBudgetID = query2.value("home_budget_calculation_id").toInt();
                }
            }
            else
            {
                error = query2.lastError().text();
            }

            query_text = "SELECT sum(amount) as amount from Fixed_expense "
                         "WHERE category_id = " + QString::number(categoryID) + " "
                         "AND home_budget_calculation = " + QString::number(homeBudgetID);

            double fixedExpense  { 0.00 };
            double oneOffExpense { 0.00 };

            qDebug() << query_text;

            query2.prepare(query_text);

            if(query2.exec())
            {
                if(query2.next())
                {
                    fixedExpense = query2.value("amount").toDouble();
                }
            }
            else
            {
                error = query2.lastError().text();
            }

            query_text = "SELECT sum(amount) as amount from One_off_expense "
                         "WHERE home_budget_id = " + QString::number(homeBudgetID) + " "
                         "AND category_id = " + QString::number(categoryID) +" ";

            qDebug() << query_text;

            query2.prepare(query_text);

            if(query2.exec())
            {
                if(query2.next())
                {
                    oneOffExpense = query2.value("amount").toDouble();
                }
            }
            else
            {
                error = query2.lastError().text();
            }

            resume.plannedExpense = fixedExpense + oneOffExpense;

            resumes.push_back(resume);

        }
    }
    else
    {
        error = query.lastError().text();
    }

    return true;
}

bool Data::loadData()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("./data_budzet.db");

    if(m_db.open())
    {
        if(loadDictionaries())
        {
            if(loadTransactions())
            {
                return true;
            }
        }

    }

    return false;

}

bool Data::loadDictionaries()
{
    if(!loadAccounts())
    {
        return false;
    }

    if(!loadMembers())
    {
        return false;
    }

    if(!loadPayees())
    {
        return false;
    }

    if(!loadCategories())
    {
        return false;
    }

    return true;
}

bool Data::loadMembers()
{
    QSqlQuery query{m_db};

    query.prepare("SELECT m.member_id, m.name FROM member m ");

    if(query.exec())
    {
        while (query.next()) {
            m_members.append(Member{
                query.value("member_id").toInt(),
                query.value("name").toString()
            });
        }

        return true;
    }

    qDebug() << query.lastError().text() << "\n" << query.lastQuery();
    return false;
}

bool Data::loadPayees()
{
    QSqlQuery query{m_db};

    query.prepare("SELECT p.payee_id, p.name FROM payee p ORDER BY p.name ");

    if(query.exec())
    {
        while (query.next()) {
            m_payees.append(Payee{
                query.value("payee_id").toInt(),
                query.value("name").toString()
            });
        }

        return true;
    }

    qDebug() << query.lastError().text() << "\n" << query.lastQuery();
    return false;
}

bool Data::loadCategories()
{
    QSqlQuery query {m_db};

    query.prepare("SELECT c.category_id , c.name FROM category c");

    if(query.exec())
    {
        while (query.next()) {

            m_categories.append(Category{
                query.value("category_id").toInt(),
                query.value("name").toString()
            });

            QSqlQuery subQuery {m_db};

            subQuery.prepare("SELECT s.subcategory_id, s.name "
                          "FROM Subcategory s "
                          "WHERE s.category_id = " +
                          QString::number(query.value("category_id").toInt()));

            if(subQuery.exec())
            {
                QVector<SubCategory> subcategories;

                while(subQuery.next())
                {
                    subcategories.append(SubCategory{
                        subQuery.value("subcategory_id").toInt(),
                        subQuery.value("name").toString(),
                        query.value("category_id").toInt()
                    });
                }

                m_subCategories.insert(query.value("category_id").toInt(),subcategories);
            }
            else
            {
                qDebug() << subQuery.lastError().text() << "\n" << subQuery.lastQuery();
                goto error;
            }

        }

        return true;
    }

    qDebug() << query.lastError().text() << "\n" << query.lastQuery();
    error:
    return false;
}

bool Data::loadAccounts()
{
    QSqlQuery query{m_db};

    query.prepare("SELECT a.account_id, a.name FROM account a ");

    if(query.exec())
    {
        while (query.next()) {
            m_accounts.append(Account{
                query.value("account_id").toInt(),
                query.value("name").toString()
            });
        }

        return true;
    }

    qDebug() << query.lastError().text() << "\n" << query.lastQuery();
    return false;
}

bool Data::loadTransactions()
{
    return true;
}
