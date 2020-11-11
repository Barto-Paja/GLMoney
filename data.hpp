#ifndef DATA_HPP
#define DATA_HPP

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

class Data : public QObject
{
    Q_OBJECT

public:
    explicit Data(QObject *parent = nullptr);

signals:

private:

    QSqlDatabase m_db;

    bool loadData();

};

#endif // DATA_HPP
