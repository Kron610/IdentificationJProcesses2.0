#include "dialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QApplication::setStyle("plastique");
//    QApplication::setStyle("motif");
//    QApplication::setStyle("cleanlooks");
//    QApplication::setStyle("cde");
//    QApplication::setStyle("windows");

    Dialog w;
    w.show();
    return a.exec();
}
