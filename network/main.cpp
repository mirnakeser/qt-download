//ako postoji error, ne ispisat headere
//headere ispisat samo jednom
//MIME



#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
