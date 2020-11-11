#include "data.hpp"



Data::Data(QObject *parent) : QObject(parent)
{

}

bool Data::loadData()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("./data_budzet.db");

    if(m_db.open())
    {

    }

    return false;

}
