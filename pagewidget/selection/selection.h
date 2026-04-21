#ifndef SELECTION_H
#define SELECTION_H

#include <skill.h>

enum depends_e
{
    OR,
    AND
};


class Selection : public QObject
{
    Q_OBJECT
public:
    Selection(page_skills_data_t* page_skills_data);
    int state_of_selection_mode = 0;
    void make_dependencies();
    void reset_skills_and_hide_unavailable_skills();
    void activate_read_page_skills(QVector<int>* active_page_skills);

public slots:
    void selection_mode_on(Skill*);
    void selection_mode_off();

signals:
    void set_scores_signal(int scores);
    void null_scores_signal();
    void activate_first_header_skill();
    void set_page_skills_selected_0_in_header_selection();

private:
    page_skills_data_t* page_skills_data;

    int num_of_available_basic_skills[12];
    int num_of_available_but_not_used_basic_skills[12];
    int num_of_skills_in_sector_active[12];

    void make_dependencies_regular_skills();
    void make_dependencies_center_skills();
    int parse_depend_type(Skill* skill);
    QVector<Skill*> parse_depends(Skill* skill);
    void add_skill_in_list(QString* cur_string_index, QVector<Skill *>* depends_list);
    Skill* find_skill_ptr_by_index(int index);
    Skill* find_regular_skill_ptr_by_index(int index);
    Skill* find_center_skill_ptr_by_index(int index);


    void select_dependencies(Skill* selected_skill);
    void unselect_dependens_skills(Skill* selected_skill);
    int  check_is_there_still_an_active_skill(Skill* related_active_skill_should_be_unselect, Skill* cur_skill);
    void select_depends_base_circle_skills(int skill_n, int sector_n);
    void unselect_depends_base_circle_skills(int skill_n, int sector_n);
    void find_skill_in_struct(Skill *selected_skill, int* ret_sector_n, sector_data_t **ret_sector,  int* ret_circle_n, int* ret_skill_i);
    void hide_of_unselect_unavailable_skills();
    short  is_skill_depends_selected(Skill* skill);
    void count_available_but_not_used_basic_skills_in_sectors();
    void count_selected_skills_in_sectors();
    void calculate_scores();
    int  find_minimum_required_base_skills(Skill* skill);
    int is_center_skill_available(Skill *skill);
    QMap<int, int> find_minimum_required_base_skills_for_center_skill(Skill* skill);
    int  calculate_num_of_selected_sectors();
    void reset_sector_base();
    void reset_active_sectors();
    void reset_hidden_skill();
    void reset_not_used_basic_skills();
    void select_first_header_skill_from_page_selection();
    void null_map(QMap<int, int>* map, int len);

    void select_dependencies_center_skill(Skill* selected_skill);

    void debug_num_of_available_basic_skills();
};

#endif // SELECTION_H
