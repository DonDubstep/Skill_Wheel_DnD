#include "selection.h"

Selection::Selection(sector_data_t *sector_ptrs[])
{
    for(int i = 0; i < 12; i++)
    {
        this->sector_ptrs[i] = sector_ptrs[i];
    }
    temp_depends_struct = new QVector<temp_depends_struct_t>();
    reset_sector_base();
    reset_active_sectors();
}

void Selection::make_dependencies()
{
    for(temp_depends_struct_t skill_el : *temp_depends_struct)
    {
        for(int d = 0; d < skill_el.depends.size(); d++)
        {
            for(temp_depends_struct_t dependend_skill : *temp_depends_struct)
            {
                if(dependend_skill.skill->index == skill_el.depends[d])
                {
                    skill_el.skill->depends.append(dependend_skill.skill);
                    break;
                }
            }
        }
    }
}

//! Закрашивает все скиллы серыми, если они не относятся к выбранному скиллу
void Selection::selection_mode_on(Skill* selected_skill)
{
    select_dependencies(selected_skill);
    count_skills_in_sectors();
    check_skills_availability();
    gray_unselected_skills();
    count_skills_in_sectors();
    check_skills_availability();
    gray_unselected_skills();
    calculate_scores();
}

void Selection::select_dependencies(Skill* selected_skill)
{
    sector_data_t *sector;
    int sector_n, circle_n, skill_n;

    find_skill_in_struct(selected_skill, &sector, &sector_n, &circle_n, &skill_n);
    if(selected_skill->state == SELECTED)
    {
        if(circle_n != 0)
        {
            num_of_available_basic_skills[sector_n]++;
            selected_skill->state = UNSELECTED;
        }
        else
        {
            if(skill_n < num_of_available_basic_skills[sector_n])
            {
                selected_skill->state = UNSELECTED;
            }
        }

    }
    else
    {
        selected_skill->state = SELECTED;
        if(circle_n != 0)
        {
            int base_skill_i = num_of_available_basic_skills[sector_n];

            sector->base_circle[base_skill_i - 1]->state = SELECTED;
            sector->base_circle[base_skill_i - 1]->repaint();
            num_of_available_basic_skills[sector_n]--;

        }
        else
        {
            for(int i = 2; i > skill_n; i--)
            {
                sector->base_circle[i]->state = SELECTED;
            }
        }

        for(Skill* related_skill : selected_skill->depends)
        {
            if(related_skill->state != SELECTED)
            {
                select_dependencies(related_skill);
            }
        }
    }

}

void Selection::find_skill_in_struct(Skill *selected_skill, sector_data_t **ret_sector, int* ret_sector_n, int* ret_circle_n, int* ret_skill_i)
{
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector = sector_ptrs[sector_i];
        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
        {
            Skill** cur_circle_ptr;
            switch (circle_i)
            {
            case 0: cur_circle_ptr = cur_sector->base_circle;   break;
            case 1: cur_circle_ptr = cur_sector->circle_1;      break;
            case 2: cur_circle_ptr = cur_sector->circle_2;      break;
            default: cur_circle_ptr = cur_sector->circle_3;     break;
            }
            for(int s = 0; s < 3; s++)
            {
                if(cur_circle_ptr[s] == selected_skill)
                {
                    *ret_sector = cur_sector;
                    *ret_sector_n = sector_i;
                    *ret_circle_n = circle_i;
                    *ret_skill_i = s;
                    return;
                }
            }
        }
    }
}

void Selection::check_skills_availability()
{
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = sector_ptrs[sector_i];
        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
        {
            Skill** cur_circle_ptr;
            switch (circle_i)
            {
            case 1: cur_circle_ptr = cur_sector_ptr->circle_1;      break;
            case 2: cur_circle_ptr = cur_sector_ptr->circle_2;      break;
            case 3: cur_circle_ptr = cur_sector_ptr->circle_3;      break;
            default: cur_circle_ptr = cur_sector_ptr->base_circle;  break;
            }
            for(int s = 0; s < 3; s++)
            {
                if(cur_circle_ptr[s]->state != SELECTED)
                {
                    int num_required_base_skill = calculate_required_base_skills_in_cur_situation(cur_circle_ptr[s]);
                    if(num_required_base_skill > num_of_available_basic_skills[sector_i] && circle_i != 0)
                    {
                        cur_circle_ptr[s]->hide();
                    }
                    else if(num_of_skills_in_sector_active[sector_i] == 0 && calculate_num_of_selected_sectors() == 5)
                    {
                        cur_circle_ptr[s]->hide();
                    }
                    else if(cur_circle_ptr[s]->isHidden())
                    {
                        cur_circle_ptr[s]->show();
                        cur_circle_ptr[s]->repaint();
                    }
                }
                else
                {
                    if(circle_i == 0)
                    {
                        for(int i = 2; i > s; i--)
                        {
                            if(cur_circle_ptr[i]->state == UNSELECTED)
                            {
                                cur_circle_ptr[s]->state = UNSELECTED;
                            }
                        }
                    }
                    else if(is_skill_depends_selected(cur_circle_ptr[s]) == 0)
                    {
                        num_of_available_basic_skills[sector_i]++;
                        cur_circle_ptr[s]->state = UNSELECTED;
                    }
                }
            }
        }
    }
}

int Selection::is_skill_depends_selected(Skill *skill)
{
    if(skill->state != SELECTED)
        return 0;
    for(Skill* related_skill : skill->depends)
    {
        if(is_skill_depends_selected(related_skill) == 0)
        {
            return 0;
        }
    }
    return 1;

}

int Selection::calculate_required_base_skills_in_cur_situation(Skill* skill)
{
    int result = 1;
    for(Skill* related_skill : skill->depends)
    {
        if(related_skill->state == SELECTED)
            continue;
        result += calculate_required_base_skills(related_skill);
    }
    return result;
}

int Selection::count_of_active_basic_skills(sector_data_t *sector)
{
    int i;
    for(i = 0; i < 3; i++)
    {
        if(sector->base_circle[i]->state == SELECTED)
            break;
    }
    return 3-i;
}

void Selection::count_skills_in_sectors()
{
    reset_active_sectors();
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector = sector_ptrs[sector_i];
        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
        {
            Skill** cur_circle_ptr;
            switch (circle_i)
            {
            case 0: cur_circle_ptr = cur_sector->base_circle;   break;
            case 1: cur_circle_ptr = cur_sector->circle_1;      break;
            case 2: cur_circle_ptr = cur_sector->circle_2;      break;
            default: cur_circle_ptr = cur_sector->circle_3;     break;
            }
            for(int s = 0; s < 3; s++)
            {
                if(cur_circle_ptr[s]->state == SELECTED)
                {
                    num_of_skills_in_sector_active[sector_i]++;
                }
            }
        }
    }
}

void Selection::calculate_scores()
{
    int result = 0;
    for(int num_of_active_skills : num_of_skills_in_sector_active)
    {
        result += num_of_active_skills;
    }
    emit set_scores_signal(result);
}

int Selection::calculate_required_base_skills(Skill* skill)
{
    int result = 1;
    for(Skill* related_skill : skill->depends)
    {
        result += calculate_required_base_skills(related_skill);
    }
    return result;
}

int Selection::calculate_num_of_selected_sectors()
{
    int counter_of_active_sectors = 0;
    for(int i = 0; i < sector_names.size(); i++)
    {
        if(num_of_skills_in_sector_active[i] != 0)
            counter_of_active_sectors++;
    }
    return counter_of_active_sectors;
}

void Selection::gray_unselected_skills()
{
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = sector_ptrs[sector_i];
        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
        {
            Skill** cur_circle_ptr;
            switch (circle_i)
            {
            case 0: cur_circle_ptr = cur_sector_ptr->base_circle;   break;
            case 1: cur_circle_ptr = cur_sector_ptr->circle_1;      break;
            case 2: cur_circle_ptr = cur_sector_ptr->circle_2;      break;
            default: cur_circle_ptr = cur_sector_ptr->circle_3;     break;
            }
            for(int s = 0; s < 3; s++)
            {
                if(cur_circle_ptr[s]->state != SELECTED && !cur_circle_ptr[s]->isHidden())
                {
                    cur_circle_ptr[s]->state = UNSELECTED;
                }
                cur_circle_ptr[s]->repaint();
            }
        }
    }
}

void Selection::reset_sector_base()
{
    for(int i = 0; i < sector_names.size(); i++)
    {
        num_of_available_basic_skills[i] = 3;
    }
}

void Selection::reset_active_sectors()
{
    for(int i = 0; i < sector_names.size(); i++)
    {
        num_of_skills_in_sector_active[i] = 0;
    }
}


////! Делает все скиллы цветными
void Selection::selection_mode_off()
{
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = sector_ptrs[sector_i];
        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
        {
            Skill** cur_circle_ptr;
            switch (circle_i)
            {
            case 0: cur_circle_ptr = cur_sector_ptr->base_circle;   break;
            case 1: cur_circle_ptr = cur_sector_ptr->circle_1;      break;
            case 2: cur_circle_ptr = cur_sector_ptr->circle_2;      break;
            default: cur_circle_ptr = cur_sector_ptr->circle_3;     break;
            }
            for(int s = 0; s < 3; s++)
            {
                cur_circle_ptr[s]->state = NONE;
                if(cur_circle_ptr[s]->isHidden())
                {
                    cur_circle_ptr[s]->show();
                }
                cur_circle_ptr[s]->repaint();
            }
        }
    }
    reset_sector_base();
    reset_active_sectors();
    emit set_scores_signal(0);
}

