#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMap>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <skill.h>
#include <QCoreApplication>
#include "skills_data.h"


struct sector_data_t
{
    Skill* base_circle[3];
    Skill* circle_1[3];
    Skill* circle_2[3];
    Skill* circle_3[3];
};

struct circle_skill_data_t
{
    sector_data_t magic_of_chaos;
    sector_data_t magic_of_darkness;
    sector_data_t magic_of_light;
    sector_data_t machine_management;
    sector_data_t education;
    sector_data_t logistics;
    sector_data_t leadership;
    sector_data_t luck;
    sector_data_t protection;
    sector_data_t attack;
    sector_data_t sorcery;
    sector_data_t summoning_magic;
};

struct temp_depends_struct_t
{
    Skill* skill;
    QVector<short> depends;
};


class PageWidget : public QWidget
{
    Q_OBJECT
public:
    PageWidget(QWidget *parent = nullptr);

private:
    int centerX;
    int centerY;
    float half_min_window_size;
    int radius1;
    int radius2;
    int radius3;
    int radius4;
    int radius5;
    int radius_small_circles;

    QPainter* painter;
    QString** segment_colors;

    circle_skill_data_t circle_skills;
    sector_data_t* sector_ptrs[12];
    QVector<temp_depends_struct_t>* temp_depends_struct;
    int num_of_available_basic_skills[12];
    int num_of_skills_in_sector_active[12];


    SkillDependencies skill_dependencies;
    const QString PIC_PATH = QCoreApplication::applicationDirPath() + "/src/HoMM5_Skills/";

    int find_in_dependent_skills(QVector<Skill*> dependent_skills, Skill* skill);
    bool eventFilter(QObject *watched, QEvent *event) override;
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void init_background_colors();
    void init_sector_pointers();
    void read_json();
    void make_dependencies();
    void paint_concentric_circles();
    void paint_small_circles();
    void paint_skills();
    void select_dependencies(Skill* selected_skill);
    void find_skill_in_struct(Skill* selected_skill, sector_data_t** ret_sector, int* ret_sector_n, int* ret_circle_n, int* ret_skill_i);
    int  is_skill_depends_selected(Skill* skill);
    void check_skills_availability();
    int  calculate_required_base_skills(Skill* skill);
    int  calculate_required_base_skills_in_cur_situation(Skill* skill);
    int  count_of_active_basic_skills(sector_data_t* sector);
    int  calculate_num_of_selected_sectors();
    void gray_unselected_skills();
    void reset_sector_base();
    void reset_active_sectors();

public slots:
    void selection_mode_on(Skill*);
    void selection_mode_off();
};

#endif // PAGEWIDGET_H
