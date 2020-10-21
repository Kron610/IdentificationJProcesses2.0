#include "getargs.h"



int indexOfPathsList(QString string)
{
    bool flag = false;
    int index = -1;
    for(int i = 0; i < string.size(); i++)
    {
        if (flag)
        {
            if (string.at(i) == ' ')
                continue;
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

QStringList getArgs(QString string)
{
    int endOf = string.indexOf("\"", 1);
    string = string.mid(endOf + 2);
    string = string.mid(indexOfPathsList(string));
    QStringList list = string.split(';');
    list.removeAll("");
    QString pathOfApp = list[0];
    return list;
}
