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

bool Data::addTransaction(const newTransaction &transaction)
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

    qDebug() << transaction.amount;

    if(query.exec())
    {
        m_db.commit();
        return true;
    }
    else
    {
        m_db.rollback();
        return false;
    }
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
