#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->installEventFilter(this);
    connect(ui->header_widget, SIGNAL(switch_page(int)), this, SLOT(change_page(int)));
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
        connect(page->selection, SIGNAL(set_scores_signal(int)), ui->header_widget, SLOT(set_scores_page(int)));
        connect(page->selection, SIGNAL(null_scores_signal()), ui->header_widget, SLOT(null_scores()));
        connect(page, SIGNAL(selection_off()), ui->header_widget->header_selection, SLOT(selection_header_off()));
    }
}

//! Функция смены вкладки для перехода между классами
void MainWindow::change_page(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}



