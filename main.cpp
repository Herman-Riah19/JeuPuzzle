#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE (image);
    QApplication a(argc, argv);
    MainWindow w;
    w.loadImage (":/moi.jpeg");
    w.show();

    return a.exec();
}
