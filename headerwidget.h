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
    QMap<QString, QVector<Skill*>> basic_skills;
    const QString PIC_PATH = QCoreApplication::applicationDirPath() + "/src/HoMM5_Skills/";
    QHBoxLayout* layout;
    void paintEvent(QPaintEvent *e) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
public:
    QComboBox* combo_pages;
//    QWidget* basic_skills_container;
    QLabel* score;
    HeaderWidget(QWidget *parent);
    void init_basic_skills();
    void read_json();
    void add_combobox();
    void add_basic_skills();
    int is_class_name_exists(QString name);
};

#endif // HEADERWIDGET_H
