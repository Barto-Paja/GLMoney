#ifndef IMPORTCSVWINDOW_HPP
#define IMPORTCSVWINDOW_HPP

#include <QDialog>
#include <QProgressDialog>
#include <QMessageBox>
#include "data.hpp"
#include <QFileDialog>
#include <QComboBox>
#include <QMap>

#include <QPair>

#include <QObject>

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

using IntsPair = QPair<int,int>;
Q_DECLARE_METATYPE(IntsPair);

class ImportCSVWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ImportCSVWindow(Data * data, QWidget *parent = nullptr);
    ~ImportCSVWindow();

public slots:
    void on_pushButton_importFile_clicked();

    void on_pushButton_commit_clicked();

    void currentIndexChanged(QString txt);

    void catchSignal(int index);

private:
    Ui::ImportCSVWindow *ui;

    Data * m_data {nullptr};

    QString m_fileName;
    QFile m_file;

    QVector<ImportedTransaction> m_imported;

    QMap<QComboBox*, QPoint> m_memberMapper;

};

#endif // IMPORTCSVWINDOW_HPP
