#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->installEventFilter(this);
    connect(ui->header_widget, SIGNAL(combobox_changed(int)), this, SLOT(change_page(int)));
    ui->tabWidget->tabBar()->setVisible(false);

    addTabs();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//! Добавляет страницы в виджет QTabWidget
void MainWindow::addTabs()
{
    for(int i = 0; i < NUM_OF_PAGES; i++)
    {
        PageWidget *page = new PageWidget();
        ui->tabWidget->addTab(page, QString::number(i+1));
    }
}

//! Функция смены вкладки для перехода между классами
void MainWindow::change_page(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}



