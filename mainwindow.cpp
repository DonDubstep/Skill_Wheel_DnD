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
    for(int i = 0; i < NUM_OF_PAGES; i++)
    {
        PageWidget *page = new PageWidget(i, this);
        pages[i] = page;
        ui->tabWidget->addTab(page, QString::number(i+1));
        connect(page->selection, SIGNAL(set_scores_signal(int)), ui->header_widget, SLOT(set_scores_page(int)));
        connect(page->selection, SIGNAL(null_scores_signal()), ui->header_widget, SLOT(null_scores()));
        connect(page->selection, SIGNAL(activate_first_header_skill()), ui->header_widget->header_selection, SLOT(select_first_header_skill()));
        connect(page, SIGNAL(selection_off()), ui->header_widget->header_selection, SLOT(selection_header_off()));
        connect(page->selection, SIGNAL(set_page_skills_selected_0_in_header_selection()), ui->header_widget->header_selection, SLOT(set_page_skills_selected_0()));
        connect(ui->header_widget, SIGNAL(set_page_skills_selected_0_in_header_selection()), ui->header_widget->header_selection, SLOT(set_page_skills_selected_0()));
    }
    preset_handler = new PresetHandler(pages, &ui->header_widget->basic_skills,this);

    QAction* openAction = new QAction("Open", this);
    QAction* saveAction = new QAction("Save", this);
    QAction* saveAsAction = new QAction("SaveAs...", this);
    openAction->setShortcut(QKeySequence::Open);
    saveAction->setShortcut(QKeySequence::Save);
    saveAsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
    this->addAction(openAction);
    this->addAction(saveAction);
    this->addAction(saveAsAction);
    connect(openAction, SIGNAL(triggered()), preset_handler, SLOT(open_preset()));
    connect(saveAction, SIGNAL(triggered()), preset_handler, SLOT(save_preset()));
    connect(saveAsAction, SIGNAL(triggered()), preset_handler, SLOT(save_as_preset()));
    connect(ui->open_action, SIGNAL(triggered()), preset_handler, SLOT(open_preset()));
    connect(ui->save_action, SIGNAL(triggered()), preset_handler, SLOT(save_preset()));
    connect(ui->save_as_action, SIGNAL(triggered()), preset_handler, SLOT(save_as_preset()));
    connect(preset_handler, SIGNAL(activate_read_skills(QVector<int>*, QVector<int>*)), this, SLOT(activate_skills_in_pages(QVector<int>*,QVector<int>*)));
}

void MainWindow::activate_skills_in_pages(QVector<int>* active_basic_skills, QVector<int>* active_page_skills)
{
    for(int p = 0; p < NUM_OF_PAGES; p++)
    {
        pages[p]->selection->activate_read_page_skills(&active_page_skills[p]);
    }
    ui->header_widget->header_selection->activate_read_basic_skills(active_basic_skills);
}

//! Функция смены вкладки для перехода между классами
void MainWindow::change_page(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}



