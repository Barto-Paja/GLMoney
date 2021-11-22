#ifndef IMPORTCSVWINDOW_HPP
#define IMPORTCSVWINDOW_HPP

#include <QDialog>
#include <QProgressDialog>
#include <QMessageBox>
#include "data.hpp"
#include <QFileDialog>
#include <QComboBox>

struct ImportedTransaction
{
    QDate date;
    QString description;
    double amount;

    ImportedTransaction() = default;
    ~ImportedTransaction() = default;
};

namespace Ui {
class ImportCSVWindow;
}

class ImportCSVWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ImportCSVWindow(Data * data, QWidget *parent = nullptr);
    ~ImportCSVWindow();

private slots:
    void on_pushButton_importFile_clicked();

    void on_pushButton_commit_clicked();

private:
    Ui::ImportCSVWindow *ui;

    Data * m_data {nullptr};

    QString m_fileName;
    QFile m_file;

    QVector<ImportedTransaction> m_imported;

};

#endif // IMPORTCSVWINDOW_HPP
