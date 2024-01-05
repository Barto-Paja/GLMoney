#include "importcsvwindow.hpp"
#include "ui_importcsvwindow.h"
#include <QTextCodec>
#include <QCheckBox>

ImportCSVWindow::ImportCSVWindow(Data *data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportCSVWindow),
    m_data { data }
{
    ui->setupUi(this);
    ui->progressBar->setHidden(true);
    ui->label_message->setHidden(true);

    for(const auto & account : m_data->accounts())
    {
        ui->comboBox_accounts->addItem(account.Name,account.ID);
    }
}

ImportCSVWindow::~ImportCSVWindow()
{
    delete ui;
}

void ImportCSVWindow::on_pushButton_importFile_clicked()
{
    ui->progressBar->setHidden(true);
    ui->label_message->setHidden(true);

    QFileDialog dialog;
    dialog.setNameFilter(tr("Pliki danych (*.csv)"));
    m_fileName = dialog.getOpenFileName();
    m_file.setFileName(m_fileName);

    if(m_file.open(QIODevice::ReadOnly))
    {
        constexpr int columns_required { 7 };
        QStringList wordList;

        while (!m_file.atEnd()) {
            QByteArray line = m_file.readLine();
            line = line.trimmed();
            QList<QByteArray> l = line.split(';');

            if(l.size() != columns_required){ continue; }

            //if(index > 0)
            //{
                ImportedTransaction t;
                long j = 0;
                for(const auto & e : qAsConst(l))
                {
                    switch(j)
                    {
                    case 0:
                    {
                        t.date = QDate::fromString(e,"yyyy-MM-dd");
                        if(!t.date.isValid()){continue;}
                        break;
                    }
                    case 1:
                    {
                        t.description = QTextCodec::codecForName("Windows-1250")->toUnicode(e);
                        break;
                    }
                    case 4:
                    {
                        auto s = e;
                        long p = s.lastIndexOf(' ');

                        QLocale pl(QLocale::Polish);

                        t.amount = pl.toDouble(s.left(p));
                        break;
                    }
                    default:
                        break;
                    }

                    j++;
                }

                m_imported.append(t);

            //}
            //index++;
        }
    }
    else
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setText("Nieduane otwarcie pliku!\n"+m_file.errorString());
        msg.addButton(QMessageBox::Ok);
        msg.exec();

        return;

    }

    ui->tableWidget->setColumnCount(8);
    QTableWidgetItem *newItem;
    newItem = new QTableWidgetItem(QString("Data Transackji"));
    ui->tableWidget->setHorizontalHeaderItem(0,newItem);
    newItem = new QTableWidgetItem(QString("Opis Importowany"));
    ui->tableWidget->setHorizontalHeaderItem(1,newItem);
    newItem = new QTableWidgetItem(QString("Kwota "));
    ui->tableWidget->setHorizontalHeaderItem(2,newItem);
    newItem = new QTableWidgetItem(QString("Kontrahent / Płatnik"));
    ui->tableWidget->setHorizontalHeaderItem(3,newItem);
    newItem = new QTableWidgetItem(QString("Centrum kosztów"));
    ui->tableWidget->setHorizontalHeaderItem(4,newItem);
    newItem = new QTableWidgetItem(QString("Kategoria"));
    ui->tableWidget->setHorizontalHeaderItem(5,newItem);
    newItem = new QTableWidgetItem(QString("Opis"));
    ui->tableWidget->setHorizontalHeaderItem(6,newItem);
    newItem = new QTableWidgetItem(QString("Czy importować?"));
    ui->tableWidget->setHorizontalHeaderItem(7,newItem);

    ui->tableWidget->setRowCount(m_imported.size());
    ui->tableWidget->update();
    ui->tableWidget->setWordWrap(true);

    long counter = 0;



    for(int index = 0; index < m_imported.size(); index++)
    {
        QTableWidgetItem *newItem;

        QComboBox * cb_payee;
        QComboBox * cb_member;
        QComboBox * cb_category;

        QCheckBox * chb;

        if(m_imported[index].date.isValid())
        {
            newItem = new QTableWidgetItem(m_imported[index].date.toString("yyyy-MM-dd"));
            newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
            ui->tableWidget->setItem(index,0,newItem);

            newItem = new QTableWidgetItem(QString("%1").arg(m_imported[index].description.trimmed()));
            newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
            ui->tableWidget->setItem(index,1,newItem);

            newItem = new QTableWidgetItem(QString("%1").arg(m_imported[index].amount));
            newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
            ui->tableWidget->setItem(index,2,newItem);

            cb_payee = new QComboBox(ui->tableWidget);
            QObject::connect(cb_payee, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ImportCSVWindow::catchSignal);

            auto payees {m_data->payees()};

            for(const auto & payee : payees )
            {
                cb_payee->addItem(payee.Name ,QVariant::fromValue(QPair<int,int>{payee.ID,payee.SuggestedSubCategoryID}));
            }

            ui->tableWidget->setCellWidget(index,3,cb_payee);

            cb_member = new QComboBox();

            auto members {m_data->members()};

            for(const auto & member : members )
            {
                cb_member->addItem(member.Name ,member.ID);
            }


            ui->tableWidget->setCellWidget(index,4,cb_member);
            m_memberMapper[cb_payee] = QPoint{index,4};

            cb_category = new QComboBox();

            auto subcategories {m_data->subCategories()};

            for(int i = 0; i < subcategories.size(); ++i)
            {
                for(const auto & subcategory : subcategories.value(i+1) )
                {
                    cb_category->addItem(subcategory.Name,subcategory.ID);
                }
            }

            ui->tableWidget->setCellWidget(index,5,cb_category);

            newItem = new QTableWidgetItem(" ");
            ui->tableWidget->setItem(index,6,newItem);

            chb = new QCheckBox();
            chb->setChecked(true);
            ui->tableWidget->setCellWidget(index,7,chb);

            counter++;


        }
    }

    ui->tableWidget->setRowCount(counter);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->update();

}

void ImportCSVWindow::on_pushButton_commit_clicked()
{
    ui->progressBar->setHidden(false);
    ui->label_message->setHidden(false);

    ui->progressBar->setMaximum(ui->tableWidget->rowCount());

    QVector<newTransaction> transactions;
    int index = 1;
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        QCheckBox * chb = qobject_cast<QCheckBox*>(ui->tableWidget->cellWidget(i,7));
        if(!chb->isChecked())
        {
            index++;
            continue;
        }

        newTransaction t;
        QTableWidgetItem* theItem = ui->tableWidget->item(i,0);
        t.date = theItem->data(0).toDate();

        theItem = ui->tableWidget->item(i,2);
        if(theItem->data(0).toDouble() < 0)
        {
            t.amount = theItem->data(0).toDouble()*(-1);
            t.multiplicand = -1;
        }
        else
        {
            t.amount = theItem->data(0).toDouble();
            t.multiplicand = 1;
        }

        QComboBox * cb = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(i,3));
        t.payeeID = cb->currentData().value<QPair<int,int>>().first;

        cb = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(i,4));
        t.memberID = cb->currentData().toInt();

        cb = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(i,5));
        t.categoryID = cb->currentData().toInt();

        theItem = ui->tableWidget->item(i,6);
        t.note = theItem->data(0).toString();

        t.accountID = ui->comboBox_accounts->currentData().toInt();

        theItem = ui->tableWidget->item(i,1);
        t.guiNote = theItem->data(0).toString();

        index++;
        transactions.append(t);


    }
    QString error;
    if(m_data->AddTransactions(transactions,error,ui->progressBar,ui->label_message))
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Information);
        msg.setText("Proces dodawania transakcji przebiegł pomyślnie! ");
        msg.addButton(QMessageBox::Ok);
        msg.exec();

        return;
    }
    else
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setText("Nieduana transakcja!\n"+error);
        msg.addButton(QMessageBox::Ok);
        msg.exec();

        return;
    }

}

void ImportCSVWindow::currentIndexChanged(QString txt)
{

}

void ImportCSVWindow::catchSignal(int index)
{
    auto xy = m_memberMapper[dynamic_cast<QComboBox*>(sender())];

    if(xy != QPoint(0,0))
    {
        QComboBox *myCB = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(xy.x(),5));
        qDebug() << dynamic_cast<QComboBox*>(sender())->currentData().value<QPair<int,int>>().first;
        auto cd = dynamic_cast<QComboBox*>(sender())->currentData().value<QPair<int,int>>().second;
        auto idx = myCB->findData(cd);

        if(idx != -1)
        {
            myCB->setCurrentIndex(idx);
        }
    }
}
