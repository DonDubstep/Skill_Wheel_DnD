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
    Skill* cur_class_skills[4];
    const QString PIC_PATH = QCoreApplication::applicationDirPath() + "/src/HoMM5_Skills/";
    QHBoxLayout* layout;
    void paintEvent(QPaintEvent *e) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void combobox_changed(int page_num);

public:
    QComboBox* combo_pages;
    QLabel* score;
    HeaderWidget(QWidget *parent);
    void init_basic_skills();
    void read_json();
    void add_combobox();
    void add_basic_skills(int page_num);
    void remove_basic_skills();
    int is_class_name_exists(QString name);

signals:
    void switch_page(int);

};

#endif // HEADERWIDGET_H
