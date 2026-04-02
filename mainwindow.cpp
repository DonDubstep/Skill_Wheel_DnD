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
    this->setWindowTitle("Skill Wheel DnD");

    addTabs();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//! Добавляет страницы в виджет QTabWidget
void MainWindow::addTabs()
{
    PageWidget* pages[NUM_OF_PAGES];
    for(int i = 0; i < NUM_OF_PAGES; i++)
    {
        PageWidget *page = new PageWidget(i);
        pages[i] = page;
        ui->tabWidget->addTab(page, QString::number(i+1));
        connect(page->selection, SIGNAL(set_scores_signal(int)), ui->header_widget, SLOT(set_scores_page(int)));
        connect(page->selection, SIGNAL(null_scores_signal()), ui->header_widget, SLOT(null_scores()));
        connect(page->selection, SIGNAL(activate_first_header_skill()), ui->header_widget->header_selection, SLOT(select_first_header_skill()));
        connect(page, SIGNAL(selection_off()), ui->header_widget->header_selection, SLOT(selection_header_off()));
        connect(page->selection, SIGNAL(set_page_skills_selected_0_in_header_selection()), ui->header_widget->header_selection, SLOT(set_page_skills_selected_0()));
        connect(ui->header_widget, SIGNAL(set_page_skills_selected_0_in_header_selection()), ui->header_widget->header_selection, SLOT(set_page_skills_selected_0()));
    }
    preset_saver = new PresetHandler(pages, &ui->header_widget->basic_skills);

    QAction *saveAction = new QAction("Save", this);
    saveAction->setShortcut(QKeySequence::Save);
    this->addAction(saveAction);
    connect(saveAction, SIGNAL(triggered()), preset_saver, SLOT(save_preset()));
}

//! Функция смены вкладки для перехода между классами
void MainWindow::change_page(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}



