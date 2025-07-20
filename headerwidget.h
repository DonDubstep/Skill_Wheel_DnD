#ifndef HEADERWIDGET_H
#define HEADERWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include "skill.h"
#include <QCoreApplication>


class HeaderWidget: public QWidget
{
Q_OBJECT
private:
    QMap<QString, Skill**> basic_skills;
    const QString PIC_PATH = QCoreApplication::applicationDirPath() + "/src/HoMM5_Skills/";
    QHBoxLayout* layout;
public:
    QComboBox* combo_pages;
//    QWidget* basic_skills_container;
    QLabel* score;
    HeaderWidget(QWidget *parent);
    void init_basic_skills();
    void read_json();
    void add_combobox();
    int is_class_name_exists(QString name);
};

#endif // HEADERWIDGET_H
