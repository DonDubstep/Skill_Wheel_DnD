#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCoreApplication>
#include <QMainWindow>
#include "skill.h"

struct skill_struct
{
    QString icon_path;
    QString title;
    QString description;
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
    void read_json();
    void show_icons();
    void paint_icons_page(QWidget* page);
    bool eventFilter(QObject* object, QEvent* event);

private:
    QVector<QString> icon_categories
    {
        "CircleBase",
        "Circle1",
        "Circle2",
        "Circle3"

    };
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
    QVector<Skill*> skills;
    QMap<QString, QVector<skill_struct>> all_skills_data;
    const QString PIC_PATH = QCoreApplication::applicationDirPath() + "/src/HoMM5_Skills/";
    Ui::MainWindow *ui;
    QLabel* background[8];

private slots:
    void change_page(int index);
public slots:
    void zoom_widget(Skill* skill);
    void zoom_out_widget(Skill* skill);

};
#endif // MAINWINDOW_H
