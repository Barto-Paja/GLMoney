#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include <QString>

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

#endif // DATASTRUCTS_H
