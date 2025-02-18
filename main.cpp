#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/src/app_icon.ico")); // Укажите путь к иконке
    return a.exec();
}
