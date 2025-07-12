#include "skills_data.h"

//! Присваивает ссылку структуры всех скиллов
void SkillDependencies::link_skill_data(QMap<QString, QVector<skill_struct> >* all_skills_data)
{
    this->all_skills_data = all_skills_data;
}

//! Рекурсивно назодит все зависимости скилла
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

//! Функция задания зависимостей между скиллами
void SkillDependencies::make_depends(QMap<QString, QVector<skill_struct> >* all_skills_data)
{
    link_skill_data(all_skills_data);
    init_skill_depends();
    // Нулевой круг
    (*all_skills_data)[icon_categories[3]][0].depends[0]  = &(*all_skills_data)[icon_categories[3]][1];
    (*all_skills_data)[icon_categories[3]][1].depends[0]  = &(*all_skills_data)[icon_categories[3]][2];
    (*all_skills_data)[icon_categories[3]][3].depends[0]  = &(*all_skills_data)[icon_categories[3]][4];
    (*all_skills_data)[icon_categories[3]][4].depends[0]  = &(*all_skills_data)[icon_categories[3]][5];
    (*all_skills_data)[icon_categories[3]][6].depends[0]  = &(*all_skills_data)[icon_categories[3]][7];
    (*all_skills_data)[icon_categories[3]][7].depends[0]  = &(*all_skills_data)[icon_categories[3]][8];
    (*all_skills_data)[icon_categories[3]][9].depends[0]  = &(*all_skills_data)[icon_categories[3]][10];
    (*all_skills_data)[icon_categories[3]][10].depends[0] = &(*all_skills_data)[icon_categories[3]][11];
    (*all_skills_data)[icon_categories[3]][12].depends[0] = &(*all_skills_data)[icon_categories[3]][13];
    (*all_skills_data)[icon_categories[3]][13].depends[0] = &(*all_skills_data)[icon_categories[3]][14];
    (*all_skills_data)[icon_categories[3]][15].depends[0] = &(*all_skills_data)[icon_categories[3]][16];
    (*all_skills_data)[icon_categories[3]][16].depends[0] = &(*all_skills_data)[icon_categories[3]][17];
    (*all_skills_data)[icon_categories[3]][18].depends[0] = &(*all_skills_data)[icon_categories[3]][19];
    (*all_skills_data)[icon_categories[3]][19].depends[0] = &(*all_skills_data)[icon_categories[3]][20];
    (*all_skills_data)[icon_categories[3]][21].depends[0] = &(*all_skills_data)[icon_categories[3]][22];
    (*all_skills_data)[icon_categories[3]][22].depends[0] = &(*all_skills_data)[icon_categories[3]][23];
    (*all_skills_data)[icon_categories[3]][24].depends[0] = &(*all_skills_data)[icon_categories[3]][25];
    (*all_skills_data)[icon_categories[3]][25].depends[0] = &(*all_skills_data)[icon_categories[3]][26];
    (*all_skills_data)[icon_categories[3]][27].depends[0] = &(*all_skills_data)[icon_categories[3]][28];
    (*all_skills_data)[icon_categories[3]][28].depends[0] = &(*all_skills_data)[icon_categories[3]][29];
    (*all_skills_data)[icon_categories[3]][30].depends[0] = &(*all_skills_data)[icon_categories[3]][31];
    (*all_skills_data)[icon_categories[3]][31].depends[0] = &(*all_skills_data)[icon_categories[3]][32];
    (*all_skills_data)[icon_categories[3]][33].depends[0] = &(*all_skills_data)[icon_categories[3]][34];
    (*all_skills_data)[icon_categories[3]][34].depends[0] = &(*all_skills_data)[icon_categories[3]][35];

    // Первый круг
    (*all_skills_data)[icon_categories[2]][0].depends[0]  = &(*all_skills_data)[icon_categories[3]][2];
    (*all_skills_data)[icon_categories[2]][1].depends[0]  = &(*all_skills_data)[icon_categories[3]][2];
    (*all_skills_data)[icon_categories[2]][2].depends[0]  = &(*all_skills_data)[icon_categories[3]][2];
    (*all_skills_data)[icon_categories[2]][3].depends[0]  = &(*all_skills_data)[icon_categories[3]][5];
    (*all_skills_data)[icon_categories[2]][4].depends[0]  = &(*all_skills_data)[icon_categories[3]][5];
    (*all_skills_data)[icon_categories[2]][5].depends[0]  = &(*all_skills_data)[icon_categories[3]][5];
    (*all_skills_data)[icon_categories[2]][6].depends[0]  = &(*all_skills_data)[icon_categories[3]][8];
    (*all_skills_data)[icon_categories[2]][7].depends[0]  = &(*all_skills_data)[icon_categories[3]][8];
    (*all_skills_data)[icon_categories[2]][8].depends[0]  = &(*all_skills_data)[icon_categories[3]][8];
    (*all_skills_data)[icon_categories[2]][9].depends[0]  = &(*all_skills_data)[icon_categories[3]][11];
    (*all_skills_data)[icon_categories[2]][10].depends[0] = &(*all_skills_data)[icon_categories[3]][11];
    (*all_skills_data)[icon_categories[2]][11].depends[0] = &(*all_skills_data)[icon_categories[3]][11];
    (*all_skills_data)[icon_categories[2]][12].depends[0] = &(*all_skills_data)[icon_categories[3]][14];
    (*all_skills_data)[icon_categories[2]][13].depends[0] = &(*all_skills_data)[icon_categories[3]][14];
    (*all_skills_data)[icon_categories[2]][14].depends[0] = &(*all_skills_data)[icon_categories[3]][14];
    (*all_skills_data)[icon_categories[2]][15].depends[0] = &(*all_skills_data)[icon_categories[3]][17];
    (*all_skills_data)[icon_categories[2]][16].depends[0] = &(*all_skills_data)[icon_categories[3]][17];
    (*all_skills_data)[icon_categories[2]][17].depends[0] = &(*all_skills_data)[icon_categories[3]][17];
    (*all_skills_data)[icon_categories[2]][18].depends[0] = &(*all_skills_data)[icon_categories[3]][20];
    (*all_skills_data)[icon_categories[2]][19].depends[0] = &(*all_skills_data)[icon_categories[3]][20];
    (*all_skills_data)[icon_categories[2]][20].depends[0] = &(*all_skills_data)[icon_categories[3]][20];
    (*all_skills_data)[icon_categories[2]][21].depends[0] = &(*all_skills_data)[icon_categories[3]][23];
    (*all_skills_data)[icon_categories[2]][22].depends[0] = &(*all_skills_data)[icon_categories[3]][23];
    (*all_skills_data)[icon_categories[2]][23].depends[0] = &(*all_skills_data)[icon_categories[3]][23];
    (*all_skills_data)[icon_categories[2]][24].depends[0] = &(*all_skills_data)[icon_categories[3]][26];
    (*all_skills_data)[icon_categories[2]][25].depends[0] = &(*all_skills_data)[icon_categories[3]][26];
    (*all_skills_data)[icon_categories[2]][26].depends[0] = &(*all_skills_data)[icon_categories[3]][26];
    (*all_skills_data)[icon_categories[2]][27].depends[0] = &(*all_skills_data)[icon_categories[3]][29];
    (*all_skills_data)[icon_categories[2]][28].depends[0] = &(*all_skills_data)[icon_categories[3]][29];
    (*all_skills_data)[icon_categories[2]][29].depends[0] = &(*all_skills_data)[icon_categories[3]][29];
    (*all_skills_data)[icon_categories[2]][30].depends[0] = &(*all_skills_data)[icon_categories[3]][32];
    (*all_skills_data)[icon_categories[2]][31].depends[0] = &(*all_skills_data)[icon_categories[3]][32];
    (*all_skills_data)[icon_categories[2]][32].depends[0] = &(*all_skills_data)[icon_categories[3]][32];
    (*all_skills_data)[icon_categories[2]][33].depends[0] = &(*all_skills_data)[icon_categories[3]][35];
    (*all_skills_data)[icon_categories[2]][34].depends[0] = &(*all_skills_data)[icon_categories[3]][35];
    (*all_skills_data)[icon_categories[2]][35].depends[0] = &(*all_skills_data)[icon_categories[3]][35];

    // Второй круг
    (*all_skills_data)[icon_categories[1]][0].depends[0]  = &(*all_skills_data)[icon_categories[3]][1];
    (*all_skills_data)[icon_categories[1]][1].depends[0]  = &(*all_skills_data)[icon_categories[3]][1];
    (*all_skills_data)[icon_categories[1]][2].depends[0]  = &(*all_skills_data)[icon_categories[3]][1];
    (*all_skills_data)[icon_categories[1]][3].depends[0]  = &(*all_skills_data)[icon_categories[3]][4];
    (*all_skills_data)[icon_categories[1]][4].depends[0]  = &(*all_skills_data)[icon_categories[3]][4];
    (*all_skills_data)[icon_categories[1]][5].depends[0]  = &(*all_skills_data)[icon_categories[3]][4];
    (*all_skills_data)[icon_categories[1]][6].depends[0]  = &(*all_skills_data)[icon_categories[3]][7];
    (*all_skills_data)[icon_categories[1]][7].depends[0]  = &(*all_skills_data)[icon_categories[3]][7];
    (*all_skills_data)[icon_categories[1]][8].depends[0]  = &(*all_skills_data)[icon_categories[3]][7];
    (*all_skills_data)[icon_categories[1]][9].depends[0]  = &(*all_skills_data)[icon_categories[3]][10];
    (*all_skills_data)[icon_categories[1]][10].depends[0] = &(*all_skills_data)[icon_categories[3]][10];
    (*all_skills_data)[icon_categories[1]][11].depends[0] = &(*all_skills_data)[icon_categories[3]][10];
    (*all_skills_data)[icon_categories[1]][12].depends[0] = &(*all_skills_data)[icon_categories[3]][13];
    (*all_skills_data)[icon_categories[1]][13].depends[0] = &(*all_skills_data)[icon_categories[3]][13];
    (*all_skills_data)[icon_categories[1]][14].depends[0] = &(*all_skills_data)[icon_categories[3]][13];
    (*all_skills_data)[icon_categories[1]][15].depends[0] = &(*all_skills_data)[icon_categories[3]][16];
    (*all_skills_data)[icon_categories[1]][16].depends[0] = &(*all_skills_data)[icon_categories[3]][16];
    (*all_skills_data)[icon_categories[1]][17].depends[0] = &(*all_skills_data)[icon_categories[3]][16];
    (*all_skills_data)[icon_categories[1]][18].depends[0] = &(*all_skills_data)[icon_categories[3]][19];
    (*all_skills_data)[icon_categories[1]][19].depends[0] = &(*all_skills_data)[icon_categories[3]][19];
    (*all_skills_data)[icon_categories[1]][20].depends[0] = &(*all_skills_data)[icon_categories[3]][19];
    (*all_skills_data)[icon_categories[1]][21].depends[0] = &(*all_skills_data)[icon_categories[3]][22];
    (*all_skills_data)[icon_categories[1]][22].depends[0] = &(*all_skills_data)[icon_categories[3]][22];
    (*all_skills_data)[icon_categories[1]][23].depends[0] = &(*all_skills_data)[icon_categories[3]][22];
    (*all_skills_data)[icon_categories[1]][24].depends[0] = &(*all_skills_data)[icon_categories[3]][25];
    (*all_skills_data)[icon_categories[1]][25].depends[0] = &(*all_skills_data)[icon_categories[3]][25];
    (*all_skills_data)[icon_categories[1]][26].depends[0] = &(*all_skills_data)[icon_categories[3]][25];
    (*all_skills_data)[icon_categories[1]][27].depends[0] = &(*all_skills_data)[icon_categories[3]][28];
    (*all_skills_data)[icon_categories[1]][28].depends[0] = &(*all_skills_data)[icon_categories[3]][28];
    (*all_skills_data)[icon_categories[1]][29].depends[0] = &(*all_skills_data)[icon_categories[3]][28];
    (*all_skills_data)[icon_categories[1]][30].depends[0] = &(*all_skills_data)[icon_categories[3]][31];
    (*all_skills_data)[icon_categories[1]][31].depends[0] = &(*all_skills_data)[icon_categories[3]][31];
    (*all_skills_data)[icon_categories[1]][32].depends[0] = &(*all_skills_data)[icon_categories[3]][31];
    (*all_skills_data)[icon_categories[1]][33].depends[0] = &(*all_skills_data)[icon_categories[3]][34];
    (*all_skills_data)[icon_categories[1]][34].depends[0] = &(*all_skills_data)[icon_categories[3]][34];
    (*all_skills_data)[icon_categories[1]][35].depends[0] = &(*all_skills_data)[icon_categories[3]][34];

    // Третий круг
    (*all_skills_data)[icon_categories[0]][0].depends[0]  = &(*all_skills_data)[icon_categories[3]][0];
    (*all_skills_data)[icon_categories[0]][1].depends[0]  = &(*all_skills_data)[icon_categories[3]][0];
    (*all_skills_data)[icon_categories[0]][2].depends[0]  = &(*all_skills_data)[icon_categories[3]][0];
    (*all_skills_data)[icon_categories[0]][3].depends[0]  = &(*all_skills_data)[icon_categories[3]][3];
    (*all_skills_data)[icon_categories[0]][4].depends[0]  = &(*all_skills_data)[icon_categories[3]][3];
    (*all_skills_data)[icon_categories[0]][5].depends[0]  = &(*all_skills_data)[icon_categories[3]][3];
    (*all_skills_data)[icon_categories[0]][6].depends[0]  = &(*all_skills_data)[icon_categories[3]][6];
    (*all_skills_data)[icon_categories[0]][7].depends[0]  = &(*all_skills_data)[icon_categories[3]][6];
    (*all_skills_data)[icon_categories[0]][8].depends[0]  = &(*all_skills_data)[icon_categories[3]][6];
    (*all_skills_data)[icon_categories[0]][9].depends[0]  = &(*all_skills_data)[icon_categories[3]][9];
    (*all_skills_data)[icon_categories[0]][10].depends[0] = &(*all_skills_data)[icon_categories[3]][9];
    (*all_skills_data)[icon_categories[0]][11].depends[0] = &(*all_skills_data)[icon_categories[3]][9];
    (*all_skills_data)[icon_categories[0]][12].depends[0] = &(*all_skills_data)[icon_categories[3]][12];
    (*all_skills_data)[icon_categories[0]][13].depends[0] = &(*all_skills_data)[icon_categories[3]][12];
    (*all_skills_data)[icon_categories[0]][14].depends[0] = &(*all_skills_data)[icon_categories[3]][12];
    (*all_skills_data)[icon_categories[0]][15].depends[0] = &(*all_skills_data)[icon_categories[3]][15];
    (*all_skills_data)[icon_categories[0]][16].depends[0] = &(*all_skills_data)[icon_categories[3]][15];
    (*all_skills_data)[icon_categories[0]][17].depends[0] = &(*all_skills_data)[icon_categories[3]][15];
    (*all_skills_data)[icon_categories[0]][18].depends[0] = &(*all_skills_data)[icon_categories[3]][18];
    (*all_skills_data)[icon_categories[0]][19].depends[0] = &(*all_skills_data)[icon_categories[3]][18];
    (*all_skills_data)[icon_categories[0]][20].depends[0] = &(*all_skills_data)[icon_categories[3]][18];
    (*all_skills_data)[icon_categories[0]][21].depends[0] = &(*all_skills_data)[icon_categories[3]][21];
    (*all_skills_data)[icon_categories[0]][22].depends[0] = &(*all_skills_data)[icon_categories[3]][21];
    (*all_skills_data)[icon_categories[0]][23].depends[0] = &(*all_skills_data)[icon_categories[3]][21];
    (*all_skills_data)[icon_categories[0]][24].depends[0] = &(*all_skills_data)[icon_categories[3]][24];
    (*all_skills_data)[icon_categories[0]][25].depends[0] = &(*all_skills_data)[icon_categories[3]][24];
    (*all_skills_data)[icon_categories[0]][26].depends[0] = &(*all_skills_data)[icon_categories[3]][24];
    (*all_skills_data)[icon_categories[0]][27].depends[0] = &(*all_skills_data)[icon_categories[3]][27];
    (*all_skills_data)[icon_categories[0]][28].depends[0] = &(*all_skills_data)[icon_categories[3]][27];
    (*all_skills_data)[icon_categories[0]][29].depends[0] = &(*all_skills_data)[icon_categories[3]][27];
    (*all_skills_data)[icon_categories[0]][30].depends[0] = &(*all_skills_data)[icon_categories[3]][30];
    (*all_skills_data)[icon_categories[0]][31].depends[0] = &(*all_skills_data)[icon_categories[3]][30];
    (*all_skills_data)[icon_categories[0]][32].depends[0] = &(*all_skills_data)[icon_categories[3]][30];
    (*all_skills_data)[icon_categories[0]][33].depends[0] = &(*all_skills_data)[icon_categories[3]][33];
    (*all_skills_data)[icon_categories[0]][34].depends[0] = &(*all_skills_data)[icon_categories[3]][33];
    (*all_skills_data)[icon_categories[0]][35].depends[0] = &(*all_skills_data)[icon_categories[3]][33];
}

//! Функция нахождения скилла в структуре всех скиллов
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

//! Заполняет нулевыми ссылками все зависимости у всех скиллов
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


//! Рекурсивный поиск всех зависимостей скилла
void SkillDependencies::dfs(skill_struct* graph, QVector<Skill *> *nodes)
{
    (*nodes).append((*graph).skill);
    if((*graph).depends[0] == nullptr) return;
    for(int i = 0; (*graph).depends[i] != nullptr; i++)
    {
        dfs((*graph).depends[i], nodes);
    }
}

