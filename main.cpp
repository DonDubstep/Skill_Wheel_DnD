#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setMinimumSize(270,300);
    w.show();
    w.setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/src/skill_wheel_icon.ico")); // Укажите путь к иконке
    return a.exec();
}
