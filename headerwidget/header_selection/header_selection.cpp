#include "header_selection.h"
#include <QDebug>

HeaderSelection::HeaderSelection(QMap<QString, QVector<Skill*>>* basic_skills)
{
    this->basic_skills = basic_skills;
}

void HeaderSelection::selection_header_on(Skill * selected_skill)
{
    select_header_dependencies(selected_skill);
    gray_unselected_header_skills();
    calculate_scores(cur_page_index);
}

void HeaderSelection::selection_header_off(QString page_name)
{
    if(page_name == "")
    {
        page_name = cur_page;
    }
    for(int i = 0; i < (*basic_skills)[page_name].size(); i++)
    {
        (*basic_skills)[page_name][i]->state = NONE;
        (*basic_skills)[page_name][i]->repaint();
    }
}


void HeaderSelection::select_first_header_skill()
{
    page_skills_selected = 1;
    selection_header_on((*basic_skills)[cur_page][0]);
}

void HeaderSelection::set_page_skills_selected_0()
{
    page_skills_selected = 0;
}

void HeaderSelection::set_cur_page(int cur_page)
{
    this->cur_page = pages[cur_page];
    this->cur_page_index = cur_page;
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
        int until_skill_index;
        // Если селекция страницы включена, то запрещаем анселектить первый скилл
        if(page_skills_selected && selected_skill_index == 0)
        {
            until_skill_index = selected_skill_index+1;
        }
        else
        {
            until_skill_index = selected_skill_index;
        }
        for(int i = (*basic_skills)[cur_page].size()-1; i >= until_skill_index; i--)
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

void HeaderSelection::calculate_scores(int page_index)
{
    int count = 0;
    QString page_name = pages[page_index];
    for(int i = 0; i < (*basic_skills)[page_name].size(); i++)
    {
        if((*basic_skills)[page_name][i]->state == SELECTED)
        {
            count++;
        }
    }
    emit set_header_scores(count, page_index);
}


bool HeaderSelection::is_skill_in_read_skills(int index, QVector<int>* active_basic_skills_in_cur_page)
{
    for(int read_s = 0; read_s < active_basic_skills_in_cur_page->size(); read_s++)
    {
        if(active_basic_skills_in_cur_page->at(read_s) == index)
        {
            return true;
        }
    }
    return false;
}

void HeaderSelection::activate_read_basic_skills(QVector<int> *active_basic_skills)
{
    QVector<int>* active_basic_skills_in_cur_page;
    for(int page_i = 0; page_i < pages.size(); page_i++)
    {
        active_basic_skills_in_cur_page = &active_basic_skills[page_i];
        QString cur_page_name = pages[page_i];
        selection_header_off(cur_page_name);
        if(active_basic_skills_in_cur_page->size() == 0)
            continue;
        for(int s = 0; s < 4; s++)
        {
            (*basic_skills)[cur_page_name][s]->state = UNSELECTED;
            if(is_skill_in_read_skills(s, active_basic_skills_in_cur_page))
            {
                (*basic_skills)[cur_page_name][s]->state = SELECTED;
            }
        }
        gray_unselected_header_skills();
        calculate_scores(page_i);
    }

}
