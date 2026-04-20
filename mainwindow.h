#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCoreApplication>
#include <QMainWindow>
#include <QEvent>
#include <QKeyEvent>
#include <pagewidget.h>
#include "headerwidget.h"
#include "preset_handler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    PresetHandler* preset_handler;
    PageWidget* pages[NUM_OF_PAGES];

private slots:
    void change_page(int index);
    void addTabs();
    void activate_skills_in_pages(QVector<int>* active_basic_skills, QVector<int>* active_page_skills);


};
#endif // MAINWINDOW_H
