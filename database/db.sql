/*
 Table Account
*/
CREATE TABLE Account (
  account_id INTEGER PRIMARY KEY ON CONFLICT ROLLBACK AUTOINCREMENT UNIQUE NOT NULL,
  name VARCHAR NOT NULL, description TEXT
);
/*
 Table Category
*/
CREATE TABLE Category (
  category_id INTEGER PRIMARY KEY ON CONFLICT ROLLBACK AUTOINCREMENT NOT NULL UNIQUE,
  name TEXT
);
-- --
INSERT INTO Category (name, category_id)
VALUES
  ('Auto i Transport', 1),
  ('Codzienne wydatki', 2),
  ('Dom', 3),
  ('Dzieci', 4),
  ('Firmowe', 5),
  ('Nieskategoryzowane', 6),
  ('Osobiste', 7),
  ('Rozrywka', 8),
  (
    'Oszczędności i inwestycje', 9
  ),
  ('Wpływy', 10);

/*
 Table Member
*/
CREATE TABLE Member (
  member_id INTEGER PRIMARY KEY ON CONFLICT ROLLBACK AUTOINCREMENT UNIQUE NOT NULL,
  name TEXT NOT NULL
);

/*
 Table Payee
*/

CREATE TABLE Payee (
  payee_id INTEGER PRIMARY KEY ON CONFLICT ROLLBACK AUTOINCREMENT UNIQUE NOT NULL,
  name TEXT NOT NULL UNIQUE ON CONFLICT ROLLBACK,
  description TEXT
);

/*
 Table Subcategory
*/

CREATE TABLE Subcategory (
  subcategory_id INTEGER PRIMARY KEY ON CONFLICT ROLLBACK AUTOINCREMENT UNIQUE NOT NULL,
  name TEXT,
  category_id NUMERIC REFERENCES Category (category_id)
);

-- --

INSERT INTO Subcategory (
  category_id, name, subcategory_id
)
VALUES
  (1, 'Paliwo', 1),
  (1, 'Parking i opłaty', 2),
  (1, 'Przejazdy', 3),
  (1, 'Serwis i części', 4),
  (1, 'Ubezpieczenie auta', 5),
  (1, 'Auto i transport - inne', 6),
  (2, 'Alkohol', 7),
  (2, 'Jedzenie poza domem', 8),
  (2, 'Papierosy', 9),
  (2, 'Zwierzęta', 10),
  (
    2, 'Żywność i chemia domowa',
    11
  ),
  (
    2, 'Codzienne wydatki - inne', 12
  ),
  (2, 'Jedzenie do pracy', 13),
  (
    3, 'Akcesoria i wyposażenie', 14
  ),
  (3, 'Remont i ogród', 15),
  (3, 'Ubezpieczenie domu', 16),
  (3, 'Usługi domowe', 17),
  (3, 'Dom - inne', 18),
  (4, 'Art. dziecięce', 19),
  (4, 'Zabawki', 20),
  (4, 'Przedszkole i opiekunka', 21),
  (4, 'Szkoła i wyprawka', 22),
  (4, 'Dzieci - inne', 23),
  (4, 'Zajęcia dodatkowe', 24),
  (
    5, 'Przelew na rachunek firmowy',
    25
  ),
  (5, 'Zakupy firmowe', 26),
  (5, 'Sprzęt', 27),
  (5, 'Wynagrodzenia', 28),
  (5, 'Firmowe - inne', 29),
  (5, 'Podatki', 30),
  (5, 'Księgowość', 31),
  (6, 'Wypłata gotówki', 32),
  (6, 'Brak kategorii', 33),
  (7, 'Edukacja', 34),
  (7, 'Elektronika', 35),
  (
    7, 'Multimedia, książki, prasa',
    36
  ),
  (7, 'Odzież i obuwie', 37),
  (7, 'Prezenty i wsparcie', 38),
  (7, 'Zdrowie i uroda', 39),
  (7, 'Osobiste - inne', 40),
  (8, 'Podróże i wyjazdy', 41),
  (8, 'Sport i hobby', 42),
  (8, 'Wyjścia i wydarzenia', 43),
  (8, 'Rozrywka - inne', 44),
  (8, 'Weekend', 45),
  (9, 'Fundusze', 46),
  (9, 'Giełda', 47),
  (
    9, 'Lokata i konto oszczędnościowe',
    48
  ),
  (9, 'Regularne oszczędzanie', 49),
  (9, 'Kruszce', 50),
  (
    9, 'Oszczędności i inwestycje - inne',
    51
  ),
  (10, 'Wynagrodzenie', 52),
  (10, 'Wpływy - inne', 53),
  (3, 'Telewizja i telefon', 54),
  (3, 'Raty i zadłużenia', 55);

/*
 Table Transaction
*/

CREATE TABLE [Transaction] (
    transaction_id INTEGER         PRIMARY KEY ON CONFLICT ROLLBACK AUTOINCREMENT
                                   UNIQUE
                                   NOT NULL,
    [when]         DATETIME        NOT NULL,
    payee_id       NUMERIC         REFERENCES Payee (payee_id),
    amount         NUMERIC (12, 3) NOT NULL,
    category_id    NUMERIC         REFERENCES Subcategory (subcategory_id),
    member_id      NUMERIC         REFERENCES Member (member_id),
    note           TEXT,
    acount_id      NUMERIC         REFERENCES Account (account_id)
                                   NOT NULL,
    multiplicand   INTEGER         NOT NULL
                                   DEFAULT ( -1)
);

/*
 Table Home_budget_calculation
*/

CREATE TABLE Home_budget_calculation (
    home_budget_calculation_id INTEGER         PRIMARY KEY ON CONFLICT ROLLBACK AUTOINCREMENT
                                               NOT NULL,
    period                     DATE            NOT NULL,
    declared_income            NUMERIC (12, 3) DEFAULT (0.0)
);

/*
 Table Fixed_expense
*/

CREATE TABLE Fixed_expense (
    fixed_expense_id        INTEGER         PRIMARY KEY ON CONFLICT ROLLBACK AUTOINCREMENT
                                            NOT NULL,
    category_id             INTEGER         REFERENCES Category (category_id),
    amount                  NUMERIC (12, 3) DEFAULT (0.0),
    home_budget_calculation INTEGER         REFERENCES Home_budget_calculation (home_budget_calculation_id)
);

/*
 Table One_off_expense
*/

CREATE TABLE One_off_expense (
    one_of_expense_id INTEGER         PRIMARY KEY AUTOINCREMENT
                                      NOT NULL,
    name              TEXT,
    amount            NUMERIC (12, 3) DEFAULT (0.0),
    home_budget_id    INTEGER         REFERENCES Home_budget_calculation (home_budget_calculation_id)
);
