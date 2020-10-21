#include "getpathofapp.h"


QString getPathOfApp(QString string)
{
    int indexOfNameApp = string.lastIndexOf("\\");
    string = string.left(indexOfNameApp + 1);
    return string;
}
