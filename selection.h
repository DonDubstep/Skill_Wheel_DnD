#ifndef SELECTION_H
#define SELECTION_H

#include <skill.h>

struct temp_depends_struct_t
{
    Skill* skill;
    QVector<short> depends;
};


class Selection : public QObject
{
    Q_OBJECT
public:
    Selection(sector_data_t* sector_ptrs[]);
    void make_dependencies();
    QVector<temp_depends_struct_t>* temp_depends_struct;

public slots:
    void selection_mode_on(Skill*);
    void selection_mode_off();

signals:
    void set_scores_signal(int scores);

private:
    sector_data_t* sector_ptrs[12];

    int num_of_available_basic_skills[12];
    int num_of_skills_in_sector_active[12];


    void select_dependencies(Skill* selected_skill);
    void find_skill_in_struct(Skill* selected_skill, sector_data_t** ret_sector, int* ret_sector_n, int* ret_circle_n, int* ret_skill_i);
    int  is_skill_depends_selected(Skill* skill);
    void check_skills_availability();
    int  calculate_required_base_skills(Skill* skill);
    int  calculate_required_base_skills_in_cur_situation(Skill* skill);
    int  count_of_active_basic_skills(sector_data_t* sector);
    void count_skills_in_sectors();
    void calculate_scores();
    int  calculate_num_of_selected_sectors();
    void gray_unselected_skills();
    void reset_sector_base();
    void reset_active_sectors();
};

#endif // SELECTION_H
