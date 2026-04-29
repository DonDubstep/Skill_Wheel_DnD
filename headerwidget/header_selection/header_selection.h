#ifndef HEADER_SELECTION_H
#define HEADER_SELECTION_H

#include <QObject>
#include "skill.h"

class HeaderSelection : public QObject
{
    Q_OBJECT
public:
    HeaderSelection(QMap<QString, QVector<Skill*>>* basic_skills);
    QMap<QString, QVector<Skill*>> *basic_skills;
    QString cur_page;
    int page_skills_selected = 0;

    void set_cur_page(int cur_page);
    void activate_read_basic_skills(QVector<int> *active_basic_skills);

public slots:
    void selection_header_on(Skill*);
    void selection_header_off(QString page_name="");
    void select_first_header_skill();
    void set_page_skills_selected_0();

signals:
    void set_header_scores(int scores);

private:
    void select_header_dependencies(Skill* selected_skill);
    void check_header_skills_availability();
    void gray_unselected_header_skills();
    int find_skill_in_header(Skill* selected_skill);
    void calculate_scores();
    bool is_skill_in_read_skills(int index, QVector<int>* active_basic_skills_in_cur_page);

};

#endif // HEADER_SELECTION_H
