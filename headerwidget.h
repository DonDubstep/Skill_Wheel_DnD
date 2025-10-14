#ifndef HEADERWIDGET_H
#define HEADERWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include "skill.h"
#include <QCoreApplication>

#define BASIC_SKILL_NUM 4

class HeaderWidget: public QWidget
{
Q_OBJECT
private:
    QMap<QString, QVector<Skill*>> basic_skills;
    Skill* cur_class_skills[BASIC_SKILL_NUM];
    QComboBox* combo_pages;
    QLabel* scores;
    const QString PIC_PATH = QCoreApplication::applicationDirPath() + "/src/HoMM5_Skills/";
    QHBoxLayout* layout;
    void paintEvent(QPaintEvent *e) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void resizeEvent(QResizeEvent* e) override;
public slots:
    void set_scores(int score);

private slots:
    void combobox_changed(int page_num);

public:
    HeaderWidget(QWidget *parent);
    void init_basic_skills();
    void read_json();
    void add_combobox();
    void add_basic_skills(int page_num);
    void add_scores(int score);
    void remove_basic_skills();
    int is_class_name_exists(QString name);
    void paint_combobox();
    void paint_basic_skills();
    void paint_scores();

signals:
    void switch_page(int);

};

#endif // HEADERWIDGET_H
