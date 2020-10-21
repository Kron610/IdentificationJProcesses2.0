#include "getappname.h"


QString getAppName(QString string)
{
    int indexOfPoint = string.lastIndexOf(".");
    string = string.mid(indexOfPoint + 1);
    int indexOfSpace = string.indexOf(' ');
    string = string.left(indexOfSpace);
    return string;
}


QString getAlternativeName(QString string)
{
    int indexOfAppName = string.lastIndexOf("\\");
    string = string.mid(indexOfAppName + 1);
    int indexOfPoint = string.lastIndexOf(".");
    string = string.left(indexOfPoint);
    int indexOfSpace = string.indexOf(' ');
    string = string.left(indexOfSpace);
    return string;
}
