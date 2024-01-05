#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include <QWidget>

namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

signals:

    void menuItem(int index);
    void quit();

private slots:
    void on_pushButton_noteIncomesAndExpenses_clicked();
    void on_pushButton_budgetPlane_clicked();

    void on_pushButton_Quit_clicked();

    void on_pushButton_About_clicked();

private:
    Ui::MainMenu *ui;
};

#endif // MAINMENU_HPP
