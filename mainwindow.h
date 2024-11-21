#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCoreApplication>
#include <QMainWindow>
#include "skill.h"

struct skill_struct
{
    QString icon_path;
    QString title;
    QString descriprion;
    QString title_color;
    Skill* skill;
};

struct all_skills_struct
{
    QVector<skill_struct> attack;
    QVector<skill_struct> battle_cries;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QVector<Skill*> skills;
    Skill* s;

    void read_json();
    void show_icons();
    all_skills_struct all_skills_data;
    QWidget* window_ptr;
    const QString PIC_PATH = QCoreApplication::applicationDirPath() + "/HoMM5_Skills/";

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
