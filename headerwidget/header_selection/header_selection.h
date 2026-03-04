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
    void set_cur_page(int cur_page);
public slots:
    void selection_header_on(Skill*);
    void selection_header_off();

signals:
    void set_header_scores(int scores);

private:
    void select_header_dependencies(Skill* selected_skill);
    void check_header_skills_availability();
    void gray_unselected_header_skills();
    int find_skill_in_header(Skill* selected_skill);
    void calculate_scores();

};

#endif // HEADER_SELECTION_H
