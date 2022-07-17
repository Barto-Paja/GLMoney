#ifndef PAYEEINFORMATIONWINDOW_HPP
#define PAYEEINFORMATIONWINDOW_HPP

#include <QDialog>
#include "data.hpp"

namespace Ui {
class PayeeInformationWindow;
}

class PayeeInformationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PayeeInformationWindow(Data * data, QWidget *parent = nullptr);
    ~PayeeInformationWindow();

private:
    Ui::PayeeInformationWindow *ui;
    Data * m_data { nullptr };
};

#endif // PAYEEINFORMATIONWINDOW_HPP
