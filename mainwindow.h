#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCoreApplication>
#include <QMainWindow>
#include <QEvent>
#include <QKeyEvent>
#include "skill.h"
#include "settings.h"
#include <pagewidget.h>

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
    QStringList pages
    {
        "Маг",
        "П. Демонов",
        "Рыцарь",
        "Некромант",
        "Рейнджер",
        "Рунный жрец",
        "Чернокнижник",
        "Варвар"
    };
    const QString PIC_PATH = QCoreApplication::applicationDirPath() + "/src/HoMM5_Skills/";
    Ui::MainWindow *ui;

private slots:
    void change_page(int index);
    void addTabs();


};
#endif // MAINWINDOW_H
