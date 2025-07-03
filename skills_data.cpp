#include "skills_data.h"
#include <QMap>
#include <QDebug>

void SkillDependencies::link_skill_data(QMap<QString, QVector<skill_struct> >* all_skills_data)
{
    this->all_skills_data = all_skills_data;
}

QVector<Skill *> SkillDependencies::show_depends(Skill *skill)
{
    QString circle;
    int index;
    find_skill(skill, &circle, &index);
    QVector<Skill *> result;
    skill_struct skill_struct_head = (*all_skills_data)[circle][index];
    dfs(&skill_struct_head, &result);

    return result;
}
void SkillDependencies::make_depends(QMap<QString, QVector<skill_struct> >* all_skills_data)
{
    link_skill_data(all_skills_data);
    init_skill_depends();
    (*all_skills_data)[icon_categories[3]][2].depends[0] = nullptr;
    (*all_skills_data)[icon_categories[3]][1].depends[0] = &(*all_skills_data)[icon_categories[3]][2];
    (*all_skills_data)[icon_categories[3]][0].depends[0] = &(*all_skills_data)[icon_categories[3]][1];
}

void SkillDependencies::find_skill(Skill *skill, QString *circle, int *index)
{
    for(int c = 0; c <= NUM_OF_CATEGORIES; c++)
    {
        QString circle_name = icon_categories[c];
        for(int i = 0; i < (*all_skills_data)[circle_name].length(); i++)
        {
            if((*all_skills_data)[circle_name][i].skill == skill)
            {
                *circle = circle_name;
                *index = i;
                return;
            }
        }
    }
}

void SkillDependencies::init_skill_depends()
{
    for(int c = 0; c < NUM_OF_CATEGORIES; c++)
    {
        QString circle = icon_categories[c];
        for(int index = 0; index < (*all_skills_data)[circle].length(); index++)
        {
            for(int d = 0; d < NUM_OF_DEPENDS; d++)
            {
                (*all_skills_data)[circle][index].depends[d] = nullptr;
            }
        }
    }
}



void SkillDependencies::dfs(skill_struct* graph, QVector<Skill *> *nodes)
{
    (*nodes).append((*graph).skill);
    if((*graph).depends[0] == nullptr) return;
    for(int i = 0; (*graph).depends[i] != nullptr; i++)
    {
        dfs((*graph).depends[i], nodes);
    }
}

