#ifndef EXPORTTOCSVWINDOW_HPP
#define EXPORTTOCSVWINDOW_HPP

#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include "data.hpp"

namespace Ui {
class ExportToCSVWindow;
}

class ExportToCSVWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ExportToCSVWindow(Data *data, QWidget *parent = nullptr);
    ~ExportToCSVWindow();

private slots:
    void on_lineEdit_fileName_textChanged(const QString &arg1);

    void on_pushButton_fileDialog_clicked();

    void on_pushButton_generate_clicked();

private:
    Ui::ExportToCSVWindow *ui;

    Data * m_data { nullptr };

    QString m_path;
    QVector<TransactionHistory> m_transactions;

    bool exportToCSV();
};

#endif // EXPORTTOCSVWINDOW_HPP
