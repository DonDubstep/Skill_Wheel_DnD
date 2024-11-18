#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    s = new Skill(this);
    QPixmap *pic = new QPixmap(QCoreApplication::applicationDirPath() + "/GraveCold_13_11zon.png");
    s->setPixmap(*pic);

}

MainWindow::~MainWindow()
{
    delete ui;
}

