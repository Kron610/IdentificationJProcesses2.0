#ifndef DIALOG_H
#define DIALOG_H

#include "getprocessbyname.h"
#include "getcommandline.h"
#include "recursivesearch.h"
#include "getappname.h"
#include "getargs.h"
#include "getpathofapp.h"
#include "file.h"

#include <QDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTime>
#include <fstream>


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

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Dialog *ui;
    QStandardItemModel* model;
    QVector<QStandardItem*>* items;
    QVector<QStringList>* names;
    bool clicked;
};
#endif // DIALOG_H
