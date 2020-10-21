#include "file.h"



QStringList getNames()
{
    QFile file("Applications.txt");
    QTextStream in(&file);

    QStringList list;
    if (!file.open(QIODevice::ReadOnly)) {
      qWarning("Cannot open file for reading");
      return list;
    }
    while (!in.atEnd())
    {
    QString line = in.readLine();
    list.append(line);
    }

    file.close();
    return list;
}



void writeNames(QStringList list)
{

    QSet<QString> set;
    QStringList stringList = getNames();
    for (auto item: stringList)
    {
        set.insert(item);
    }

    for (auto item: list)
    {
        set.insert(item);
    }

    QFile file("Applications.txt");

    QTextStream out(&file);

    if (file.open(QIODevice::WriteOnly))
    {
        for (auto string: set)
        {
            out << string << endl;
        }
    }

    file.close();
}
