#ifndef SKILLSDATA_H
#define SKILLSDATA_H

#include "skill.h"

class SkillDependencies
{
public:
    void make_depends(QMap<QString, QVector<skill_struct> >* all_skills_data);
    QVector<Skill*> show_depends(Skill* skill);
private:
    //! Ссылка на структуру со всеми скиллами
    QMap<QString, QVector<skill_struct> >* all_skills_data;

    void link_skill_data(QMap<QString, QVector<skill_struct> >* all_skills_data);
    void find_skill(Skill *skill, QString *circle, int *index);
    void init_skill_depends();
    void dfs(skill_struct* graph, QVector<Skill*> *nodes);
    void make_depends_circle_0(QMap<QString, QVector<skill_struct> >* all_skills_data);
    void make_depends_circle_1_3(QMap<QString, QVector<skill_struct> >* all_skills_data);
//    void make_optional_depends(QMap<QString, QVector<skill_struct> >* all_skills_data);
};

#endif // SKILLSDATA_H
