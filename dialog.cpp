#include "dialog.h"
#include "ui_dialog.h"




Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    names = new QVector<QStringList>();
    items = new QVector<QStandardItem*>();
    QStringList namesList;

    clicked = false;
    std::vector<int> keys;
    std::vector<DWORD> ids = GetProcessByName(L"javaw.exe", L"java.exe", keys);
    model = new QStandardItemModel(ids.size(), 4, this);

    ui->tableView->setModel(model);
    ui->progressBar->setRange(0, ids.size());

    ui->tableView->setColumnWidth(0, 300);
    ui->tableView->setColumnWidth(2, 150);
    ui->tableView->setColumnWidth(3, 300);

    QModelIndex index;
    int i = 0;
    for (auto dwId: ids)
    {

        PWSTR buf;
        get_cmd_line(dwId, buf);

        QString string = QString::fromWCharArray(buf);
        std::ofstream out("cmdLine.txt");
        out << string.toStdString();
        out.close();
        QStringList list = getArgs(string);
        QString pathOfApp = getPathOfApp(list[0]);
        QString appName = getAppName(list.last());
        namesList.append(appName);

        QStringList nameList = QStringList();
        QString altName = getAlternativeName(list[0]);
        nameList.push_back(altName);
        altName = getAlternativeName(list[1]);
        nameList.push_back(altName);
        names->append(nameList);

        auto wPath = pathOfApp.toStdWString();

        auto images = QStringList();
        bool flag = false;
        enumsubfiles(wPath, L"*.ico", 1, ENUM_FILE, [&images, &flag](const std::wstring &dir_with_back_slant, _wfinddata_t &attrib)->bool
            {
                    QString imageName = QString::fromStdWString(dir_with_back_slant + attrib.name);
                    images.append(imageName);
                    flag = true;
                return true;
            });

        if (!flag)
        {
            pathOfApp.chop(1);
            int indexOfPrevPath = pathOfApp.lastIndexOf("\\");
            pathOfApp = pathOfApp.left(indexOfPrevPath + 1);
            wPath = pathOfApp.toStdWString();
            enumsubfiles(wPath, L"*.ico", 2, ENUM_FILE, [&images, &flag](const std::wstring &dir_with_back_slant, _wfinddata_t &attrib)->bool
                {
                        QString imageName = QString::fromStdWString(dir_with_back_slant + attrib.name);
                        images.append(imageName);
                        flag = true;
                    return true;
                });
        }

        QStandardItem* item = new QStandardItem();
        item->setText(appName);
        QIcon icon;
        if (images.empty())
            icon.addFile("standard_icon.ico");
        else
            icon.addFile(images[0]);
        item->setIcon(icon);
        items->push_back(item);
        index = model->index(i, 1);
        model->setData(index, static_cast<int>(dwId));
        model->setItem(i, 0, items->at(i));
        index = model->index(i, 2);

        if(keys.at(i) == 0)
            model->setData(index, "javaw.exe");
        if(keys.at(i) == 1)
            model->setData(index, "java.exe");

        index = model->index(i++, 3);
        model->setData(index, pathOfApp);

        ui->progressBar->setValue(i);

    }

    model->setHeaderData(0,Qt::Horizontal, "Java-Process");
    model->setHeaderData(1,Qt::Horizontal, "ID");
    model->setHeaderData(2,Qt::Horizontal, "Parent process");
    model->setHeaderData(3,Qt::Horizontal, "Path");

    writeNames(namesList);
    ui->progressBar->hide();
    ui->timeEdit->setTime(QTime::currentTime());

}

Dialog::~Dialog()
{
    delete ui;
    items->clear();
    names->clear();
}


void Dialog::on_updatePushButton_clicked()
{
    names->clear();
    items->clear();
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);
    QStringList namesList;

    std::vector<int> keys;
    std::vector<DWORD> ids = GetProcessByName(L"javaw.exe", L"java.exe", keys);
    items->clear();
    model->setRowCount(ids.size());
    int i = 0;
    QModelIndex index;
    for (auto dwId: ids)
    {

        PWSTR buf;
        get_cmd_line(dwId, buf);

        QString string = QString::fromWCharArray(buf);

        QStringList list = getArgs(string);
        QString pathOfApp = getPathOfApp(list[0]);
        QString appName = getAppName(list.last());
        namesList.append(appName);

        QStringList nameList = QStringList();
        QString altName = getAlternativeName(list[0]);
        nameList.push_back(altName);
        altName = getAlternativeName(list[1]);
        nameList.push_back(altName);
        names->append(nameList);

        auto wPath = pathOfApp.toStdWString();

        auto images = QStringList();
        bool flag = false;
        enumsubfiles(wPath, L"*.ico", 1, ENUM_FILE, [&images, &flag](const std::wstring &dir_with_back_slant, _wfinddata_t &attrib)->bool
            {
                    QString imageName = QString::fromStdWString(dir_with_back_slant + attrib.name);
                    images.append(imageName);
                    flag = true;
                return true;
            });

        if (!flag)
        {
            pathOfApp.chop(1);
            int indexOfPrevPath = pathOfApp.lastIndexOf("\\");
            pathOfApp = pathOfApp.left(indexOfPrevPath + 1);
            wPath = pathOfApp.toStdWString();
            enumsubfiles(wPath, L"*.ico", 2, ENUM_FILE, [&images, &flag](const std::wstring &dir_with_back_slant, _wfinddata_t &attrib)->bool
                {
                        QString imageName = QString::fromStdWString(dir_with_back_slant + attrib.name);
                        images.append(imageName);
                        flag = true;
                    return true;
                });
        }

        QStandardItem* item = new QStandardItem();
        if (clicked)
        {
            appName = appName + " (" +  names->back().at(0) + ", " + names->back().at(1) + ")";
        }
        item->setText(appName);
        QIcon icon;
        if (images.empty())
            icon.addFile("standard_icon.ico");
        else
            icon.addFile(images[0]);
        item->setIcon(icon);
        items->push_back(item);
        model->setItem(i, 0, items->at(i));
        index = model->index(i, 1);
        model->setData(index, static_cast<int>(dwId));


        index = model->index(i, 2);

        if(keys.at(i) == 0)
            model->setData(index, "javaw.exe");
        if(keys.at(i) == 1)
            model->setData(index, "java.exe");

        index = model->index(i++, 3);
        model->setData(index, pathOfApp);

        ui->progressBar->setValue(i);

    }

    writeNames(namesList);
    ui->progressBar->hide();

    ui->timeEdit->setTime(QTime::currentTime());

}

void Dialog::on_progressBar_valueChanged(int value)
{
}



void Dialog::on_pushButton_clicked()
{
    if (!clicked)
    {
        ui->pushButton->setStyleSheet("border-radius: 10px; \
                                      background-color: rgb(224, 57, 57); \
                                      border: 1px solid rgb(222, 222, 222); \
                                      height: 30px; \
                                      font-size: 14px; \
                                      font-weight: 600; \
                                      color: rgb(49, 49, 49); \
                                      border-color:rgb(49, 49, 49);");

                                      clicked = true;
                                      ui->pushButton->setText("Hide alternative names");

                                      for (int i = 0; i < items->size(); i++)
                                      {
                                      QString string = items->at(i)->text();
                                      string = string + " (" + names->at(i).at(0) + ", " + names->at(i).at(1) + ")";
                                      items->at(i)->setText(string);
                                      }

    }
    else
    {
        ui->pushButton->setStyleSheet("border-radius: 10px; \
                                      background-color:rgb(85, 255, 127); \
                                      border: 1px solid rgb(222, 222, 222); \
                                      height: 30px; \
                                      font-size: 14px; \
                                      font-weight: 600; \
                                      color: rgb(49, 49, 49); \
                                      border-color:rgb(49, 49, 49);");

                                      clicked = false;
                                      ui->pushButton->setText("Show alternative names");

                                      for (int i = 0; i < items->size(); i++)
                                      {
                                      QString string = items->at(i)->text();
                                      int indexOfAltNames = string.indexOf("(");
                                      string = string.left(indexOfAltNames-1);
                                      items->at(i)->setText(string);
                                      }

    }
}

void Dialog::on_pushButton_2_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Applications");
    msgBox.setStyleSheet("border-radius: 10px; \
                         background-color: none; \
                         font-size: 14px; \
                         font-weight: 600; \
                         color: rgb(49, 49, 49);");
    QStringList names = getNames();
    QString string = "";
    for (auto item: names)
    {
        string += item + "\n";
    }
    msgBox.setText(string);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}
