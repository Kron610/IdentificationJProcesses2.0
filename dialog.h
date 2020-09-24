#ifndef DIALOG_H
#define DIALOG_H

#include "getprocessbyname.h"
#include "getcommandline.h"
#include "recursivesearch.h"

#include <QDialog>
#include <QStandardItemModel>
#include <QString>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();


private slots:
    void on_updatePushButton_clicked();

    void on_progressBar_valueChanged(int value);

private:
    Ui::Dialog *ui;
    QStandardItemModel* model;
    QVector<QStandardItem*>* items;
};
#endif // DIALOG_H
