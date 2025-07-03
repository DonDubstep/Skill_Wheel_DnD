#ifndef SKILLSDATA_H
#define SKILLSDATA_H

#include "skill.h"

class SkillDependencies
{
public:
    void make_depends(QMap<QString, QVector<skill_struct> >* all_skills_data);
    QVector<Skill*> show_depends(Skill* skill);
    void link_skill_data(QMap<QString, QVector<skill_struct> >* all_skills_data);
    QMap<QString, QVector<skill_struct> >* all_skills_data;
private:
    void find_skill(Skill *skill, QString *circle, int *index);
    void init_skill_depends();
    void dfs(skill_struct* graph, QVector<Skill*> *nodes);
};

#endif // SKILLSDATA_H
