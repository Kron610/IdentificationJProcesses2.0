#include "dialog.h"
#include "ui_dialog.h"

int indexOfPathsList(QString string)
{
    bool flag = false;
    int index = -1;
    for(int i = 0; i < string.size(); i++)
    {
        if (flag)
        {
            if (string.at(i) != '-')
            {
                index = i;
                if (string.at(i) == "\"")
                    index++;
                break;
            }
            flag = false;
            continue;
        }
        if (string.at(i) == ' ')
        {
            flag = true;
            continue;
        }
    }
    return index;
}


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    items = new QVector<QStandardItem*>();
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

        int endOf = string.indexOf("\"", 1);
        QString mainPath = string.mid(1, endOf - 1);
        string = string.mid(endOf + 2);
        string = string.mid(indexOfPathsList(string));
        QStringList list = string.split(';');
        list.removeAll("");
        QString pathOfApp = list[0];
        int indexOfNameApp = pathOfApp.lastIndexOf("\\");
        int indexOfFormat = pathOfApp.lastIndexOf(".");
        QString appName = pathOfApp.mid(indexOfNameApp + 1, indexOfFormat - indexOfNameApp - 1);
        pathOfApp = pathOfApp.left(indexOfNameApp + 1);
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

   // ui->progressBar->hide();
    ui->timeEdit->setTime(QTime::currentTime());

}

Dialog::~Dialog()
{
    delete ui;
    items->clear();
}


void Dialog::on_updatePushButton_clicked()
{
    items->clear();
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);

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

        int endOf = string.indexOf("\"", 1);
        QString mainPath = string.mid(1, endOf - 1);
        string = string.mid(endOf + 2);
        string = string.mid(indexOfPathsList(string));
        QStringList list = string.split(';');
        list.removeAll("");
        QString pathOfApp = list[0];
        int indexOfNameApp = pathOfApp.lastIndexOf("\\");
        int indexOfFormat = pathOfApp.lastIndexOf(".");
        QString appName = pathOfApp.mid(indexOfNameApp + 1, indexOfFormat - indexOfNameApp - 1);
        pathOfApp = pathOfApp.left(indexOfNameApp + 1);
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

    ui->progressBar->hide();

    ui->timeEdit->setTime(QTime::currentTime());

}

void Dialog::on_progressBar_valueChanged(int value)
{
}
