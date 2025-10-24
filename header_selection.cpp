#include "header_selection.h"

HeaderSelection::HeaderSelection(QMap<QString, QVector<Skill*>>* basic_skills)
{
    this->basic_skills = basic_skills;
}
#include <QDebug>
void HeaderSelection::selection_header_on(Skill * selected_skill)
{
    select_header_dependencies(selected_skill);
    gray_unselected_header_skills();
    calculate_scores();
}

void HeaderSelection::selection_header_off()
{
    for(int i = 0; i < (*basic_skills)[cur_page].size(); i++)
    {
        (*basic_skills)[cur_page][i]->state = NONE;
        (*basic_skills)[cur_page][i]->repaint();
    }
}

void HeaderSelection::set_cur_page(int cur_page)
{
    this->cur_page = pages[cur_page];
}

void HeaderSelection::select_header_dependencies(Skill *selected_skill)
{
    int selected_skill_index = find_skill_in_header(selected_skill);
    if(selected_skill->state != SELECTED)
    {
        for(int i = selected_skill_index; i >= 0; i--)
        {
            (*basic_skills)[cur_page][i]->state = SELECTED;
        }
    }
    else
    {
        for(int i = (*basic_skills)[cur_page].size()-1; i >= selected_skill_index; i--)
        {
            (*basic_skills)[cur_page][i]->state = UNSELECTED;
        }
    }

}

void HeaderSelection::gray_unselected_header_skills()
{
    for(int i = 0; i < (*basic_skills)[cur_page].size(); i++)
    {
        if((*basic_skills)[cur_page][i]->state != SELECTED)
        {
            (*basic_skills)[cur_page][i]->state = UNSELECTED;
        }
        (*basic_skills)[cur_page][i]->repaint();
    }
}

int HeaderSelection::find_skill_in_header(Skill *selected_skill)
{
    for(int i = 0; i < (*basic_skills)[cur_page].size(); i++)
    {
        if((*basic_skills)[cur_page][i] == selected_skill)
        {
            return i;
        }
    }
}

void HeaderSelection::calculate_scores()
{
    int count = 0;
    for(int i = 0; i < (*basic_skills)[cur_page].size(); i++)
    {
        if((*basic_skills)[cur_page][i]->state == SELECTED)
        {
            count++;
        }
    }
    emit set_header_scores(count);
}
