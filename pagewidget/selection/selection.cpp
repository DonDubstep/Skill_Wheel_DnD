#include "selection.h"
#include <QDebug>

Selection::Selection(sector_data_t *sector_ptrs[])
{
    for(int i = 0; i < 12; i++)
    {
        this->sector_ptrs[i] = sector_ptrs[i];
    }
    reset_sector_base();
    reset_active_sectors();
}

void Selection::make_dependencies()
{
    Skill* cur_skill;
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = sector_ptrs[sector_i];
        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
        {
            Skill** cur_circle_ptr;
            switch (circle_i)
            {
            case CIRCLE_1:  cur_circle_ptr = cur_sector_ptr->circle_1;      break;
            case CIRCLE_2:  cur_circle_ptr = cur_sector_ptr->circle_2;      break;
            case CIRCLE_3:  cur_circle_ptr = cur_sector_ptr->circle_3;      break;
            default:        cur_circle_ptr = cur_sector_ptr->base_circle;   break;
            }
            for(int s = 0; s < 3; s++)
            {
                cur_skill = cur_circle_ptr[s];
                cur_skill->depend_type = parse_depend_type(cur_skill);
                cur_skill->depends = parse_depends(cur_skill);
            }
        }
    }
}

int Selection::parse_depend_type(Skill* skill)
{
    QString parsed_depends = skill->parsed_depends;
    if(parsed_depends.contains("&"))
    {
        return AND;
    }
    else if (parsed_depends.contains("|"))
    {
        return OR;
    }
    else
    {
        return AND;
    }
}

QVector<Skill *> Selection::parse_depends(Skill* skill)
{
    QString cur_string_index = "";
    QVector<Skill *>* ret_depends_list = new QVector<Skill *>();
    if(skill->parsed_depends == "")
        return *ret_depends_list;

    QChar last_letter;
    for(QChar letter : skill->parsed_depends)
    {
        if(letter == " ")
        {
            add_skill_in_list(&cur_string_index, ret_depends_list);
            continue;
        }
        else if(letter.isDigit())
        {
            cur_string_index += letter;
        }
        else if(letter == "|" || letter == "&")
        {
            add_skill_in_list(&cur_string_index, ret_depends_list);
        }
        last_letter = letter;
    }
    add_skill_in_list(&cur_string_index, ret_depends_list);
    return *ret_depends_list;
}


void Selection::add_skill_in_list(QString* cur_string_index, QVector<Skill *>* ret_depends_list)
{
    if(*cur_string_index != "")
    {
        int cur_skill_index = cur_string_index->toInt();
        Skill* cur_skill = find_skill_ptr_by_index(cur_skill_index);
        ret_depends_list->append(cur_skill);
        *cur_string_index = "";
    }
}


Skill *Selection::find_skill_ptr_by_index(int index)
{
    Skill* cur_skill;
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = sector_ptrs[sector_i];
        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
        {
            Skill** cur_circle_ptr;
            switch (circle_i)
            {
            case CIRCLE_1:  cur_circle_ptr = cur_sector_ptr->circle_1;      break;
            case CIRCLE_2:  cur_circle_ptr = cur_sector_ptr->circle_2;      break;
            case CIRCLE_3:  cur_circle_ptr = cur_sector_ptr->circle_3;      break;
            default:        cur_circle_ptr = cur_sector_ptr->base_circle;   break;
            }
            for(int s = 0; s < 3; s++)
            {
                cur_skill = cur_circle_ptr[s];
                if(cur_skill->index == index)
                {
                    return cur_skill;
                }
            }
        }
    }
    qDebug() << "Не найден скилл по индексу " << index;
    return nullptr;
}


//! Закрашивает все скиллы серыми, если они не относятся к выбранному скиллу
void Selection::selection_mode_on(Skill* selected_skill)
{
    select_dependencies(selected_skill);
    reset_hidden_skill();
    count_selected_skills_in_sectors();
    count_available_but_not_used_basic_skills_in_sectors();
    hide_of_unselect_unavailable_skills();
    calculate_scores();
}

void Selection::select_dependencies(Skill* selected_skill)
{
    sector_data_t *sector;
    int sector_n = 0, circle_n = 0, skill_n = 0;
    find_skill_in_struct(selected_skill, &sector, &sector_n, &circle_n, &skill_n);
    if(selected_skill->state == SELECTED)
    {
        if(circle_n != BASE_CIRCLE)
        {
            selected_skill->state = UNSELECTED;
            selected_skill->repaint();
            unselect_dependens_skills(selected_skill);
        }
        else
        {
            if(num_of_available_but_not_used_basic_skills[sector_n] != 0)
            {
                unselect_depends_base_circle_skills(skill_n, sector_n);
            }
        }
    }
    else
    {
        if(circle_n != BASE_CIRCLE)
        {
            selected_skill->state = SELECTED;
            selected_skill->repaint();
            if(num_of_available_but_not_used_basic_skills[sector_n] > 0)
            {
                num_of_available_but_not_used_basic_skills[sector_n]--;
            }
            else
            {
                int base_skill_i = num_of_available_basic_skills[sector_n] - 1;

                sector->base_circle[base_skill_i]->state = SELECTED;
                sector->base_circle[base_skill_i]->repaint();
                num_of_available_basic_skills[sector_n]--;
            }
        }
        else
        {
            select_depends_base_circle_skills(skill_n, sector_n);
        }

        if(selected_skill->depend_type == AND)
        {
            for(Skill* related_skill : selected_skill->depends)
            {
                if(related_skill->state != SELECTED)
                {
                    select_dependencies(related_skill);
                }
            }
        }
        else
        {
            bool found_selected_related_skill = false;
            for(Skill* related_skill : selected_skill->depends)
            {
                if (related_skill->state == SELECTED)
                {
                    found_selected_related_skill = true;
                    break;
                }
            }
            if(!found_selected_related_skill)
            {
                Skill* chosen_related_skill;
                int min_distance = -1;
                int required_distance = -1;
                for(Skill* related_skill : selected_skill->depends)
                {
                    required_distance = find_minimum_required_base_skills(related_skill);
                    if(required_distance < min_distance || min_distance == -1)
                    {
                        min_distance = required_distance;
                        chosen_related_skill = related_skill;
                    }
                }
                select_dependencies(chosen_related_skill);
            }
        }
    }

}

//! Здесь выбираем серым все скиллы, в зависимостях у которых есть наш скилл
void Selection::unselect_dependens_skills(Skill *selected_skill)
{
    static QVector<Skill*> new_unselected_skills;
    Skill* cur_skill;
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = sector_ptrs[sector_i];
        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
        {
            Skill** cur_circle_ptr;
            switch (circle_i)
            {
            case CIRCLE_1:  cur_circle_ptr = cur_sector_ptr->circle_1;      break;
            case CIRCLE_2:  cur_circle_ptr = cur_sector_ptr->circle_2;      break;
            case CIRCLE_3:  cur_circle_ptr = cur_sector_ptr->circle_3;      break;
            default:        cur_circle_ptr = cur_sector_ptr->base_circle;   break;
            }
            for(int s = 0; s < 3; s++)
            {
                cur_skill = cur_circle_ptr[s];
                //! Для зависимости типа И все зависимые скиллы отключаем и заносим в список для повторного прогона функции
                if(cur_skill->depend_type == AND)
                {
                    for(Skill* related_skill : cur_skill->depends)
                    {
                        if(related_skill == selected_skill)
                        {
                            qDebug() << "unselect_dependens_skills для скилла" << selected_skill->index;
                            cur_skill->state = UNSELECTED;
                            cur_skill->repaint();
                            new_unselected_skills.append(cur_skill);
                        }
                    }
                }
                //! Для зависимости типа ИЛИ проверяем, есть ли кроме нашего выбранного скилла ещё зависимые скиллы которые включены,
                //! если нет - отключаем и заносим в список для повторного прогона функции
                else if(cur_skill->depend_type == OR)
                {
                    for(Skill* related_skill : cur_skill->depends)
                    {
                        if(related_skill == selected_skill)
                        {
                            if(check_is_there_still_an_active_skill(related_skill, cur_skill) == 0)
                            {
                                cur_skill->state = UNSELECTED;
                                cur_skill->repaint();
                                new_unselected_skills.append(cur_skill);
                            }
                        }
                    }
                }
            }
        }
    }
    if(new_unselected_skills.size() != 0)
    {
        qDebug() << "new_unselected_skills =" << new_unselected_skills.last()->index;
        Skill* new_unseleted_skill = new_unselected_skills.last();
        new_unselected_skills.pop_back();
        unselect_dependens_skills(new_unseleted_skill);
    }
}

//! Проверка, есть ли ещё зависимые скиллы, которые активированы.
//! Сделано для зависимости по ИЛИ, чтоб отключить скилл, только в случае отключения всех его зависимостей
int Selection::check_is_there_still_an_active_skill(Skill *related_active_skill_should_be_unselect, Skill *cur_skill)
{
    for(Skill* related_skill : cur_skill->depends)
    {
        if(related_skill == related_active_skill_should_be_unselect)
        {
            continue;
        }
        if(related_skill->state == SELECTED)
        {
            return 1;
        }
    }
    return 0;
}

void Selection::select_depends_base_circle_skills(int skill_n, int sector_n)
{
    for(;skill_n < 3; skill_n++)
    {
        Skill* cur_skill = sector_ptrs[sector_n]->base_circle[skill_n];
        if(cur_skill->state != SELECTED)
        {
            cur_skill->state = SELECTED;
            cur_skill->repaint();
            num_of_available_basic_skills[sector_n]--;
        }
    }
}

void Selection::unselect_depends_base_circle_skills(int skill_n, int sector_n)
{
    //! Порядок базовых скиллов следующий: первый базовый скилл 2, второй базовый скилл - 1, третий - 0
    for(int s = skill_n; s >=0; s--)
    {
        Skill* cur_skill = sector_ptrs[sector_n]->base_circle[s];
        if(cur_skill->state == SELECTED)
        {
            cur_skill->state = UNSELECTED;
            cur_skill->repaint();
            num_of_available_basic_skills[sector_n]++;
        }
    }
}

void Selection::find_skill_in_struct(Skill *selected_skill, sector_data_t **ret_sector, int* ret_sector_n, int* ret_circle_n, int* ret_skill_i)
{
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = sector_ptrs[sector_i];
        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
        {
            Skill** cur_circle_ptr;
            switch (circle_i)
            {
            case CIRCLE_1:  cur_circle_ptr = cur_sector_ptr->circle_1;      break;
            case CIRCLE_2:  cur_circle_ptr = cur_sector_ptr->circle_2;      break;
            case CIRCLE_3:  cur_circle_ptr = cur_sector_ptr->circle_3;      break;
            default:        cur_circle_ptr = cur_sector_ptr->base_circle;   break;
            }
            for(int s = 0; s < 3; s++)
            {
                if(cur_circle_ptr[s] == selected_skill)
                {
                    *ret_sector = cur_sector_ptr;
                    *ret_sector_n = sector_i;
                    *ret_circle_n = circle_i;
                    *ret_skill_i = s;
                    return;
                }
            }
        }
    }
}

void Selection::hide_of_unselect_unavailable_skills()
{
    Skill* cur_skill;
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = sector_ptrs[sector_i];
        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
        {
            Skill** cur_circle_ptr;
            switch (circle_i)
            {
            case CIRCLE_1:  cur_circle_ptr = cur_sector_ptr->circle_1;      break;
            case CIRCLE_2:  cur_circle_ptr = cur_sector_ptr->circle_2;      break;
            case CIRCLE_3:  cur_circle_ptr = cur_sector_ptr->circle_3;      break;
            default:        cur_circle_ptr = cur_sector_ptr->base_circle;   break;
            }
            for(int s = 0; s < 3; s++)
            {
                cur_skill = cur_circle_ptr[s];
                if(cur_skill->state != SELECTED)
                {
                    int num_required_base_skill = find_minimum_required_base_skills(cur_skill);
                    if(num_required_base_skill > (num_of_available_basic_skills[sector_i] + num_of_available_but_not_used_basic_skills[sector_i]) &&
                       circle_i != BASE_CIRCLE)
                    {
                        cur_skill->state = HIDDEN;
                        cur_skill->hide();
                    }
                    else if(num_of_skills_in_sector_active[sector_i] == 0 && calculate_num_of_selected_sectors() >= 5)
                    {
                        cur_skill->state = HIDDEN;
                        cur_skill->hide();
                    }
                    else if(is_skill_depends_selected(cur_skill) == 0)
                    {
                        cur_skill->state = UNSELECTED;
                        cur_skill->repaint();
                    }
                    else
                    {
                        qDebug() << "Где я?" << cur_skill->index;
                    }
                }
            }
        }
    }
}

void Selection::reset_skills_and_hide_unavailable_skills()
{
    Skill* cur_skill;
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = sector_ptrs[sector_i];
        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
        {
            Skill** cur_circle_ptr;
            switch (circle_i)
            {
            case CIRCLE_1:  cur_circle_ptr = cur_sector_ptr->circle_1;      break;
            case CIRCLE_2:  cur_circle_ptr = cur_sector_ptr->circle_2;      break;
            case CIRCLE_3:  cur_circle_ptr = cur_sector_ptr->circle_3;      break;
            default:        cur_circle_ptr = cur_sector_ptr->base_circle;  break;
            }
            for(int s = 0; s < 3; s++)
            {
                cur_skill = cur_circle_ptr[s];
                int num_required_base_skill = find_minimum_required_base_skills(cur_skill);
                if(num_required_base_skill > num_of_available_basic_skills[sector_i] && circle_i != BASE_CIRCLE)
                {
                    cur_skill->state = HIDDEN;
                    cur_skill->hide();
                }
                else if(num_of_skills_in_sector_active[sector_i] == 0 && calculate_num_of_selected_sectors() >= 5)
                {
                    cur_skill->state = HIDDEN;
                    cur_skill->hide();
                }
                else
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
    }
}

//! Проверка все ли зависимые скиллы уже выбраны
int Selection::is_skill_depends_selected(Skill *skill)
{
    if(skill->state != SELECTED)
        return 0;
    for(int s = 0; s < skill->depends.size(); s++)
    {
        Skill* related_skill = skill->depends[s];
        if(skill->depend_type == AND)
        {
            if(is_skill_depends_selected(related_skill) == 0)
            {
                return 0;
            }
            else
            {
                if(s == skill->depends.size()-1)
                {
                    return 1;
                }
                else
                {
                    continue;
                }
            }
        }
        else if(skill->depend_type == OR)
        {
            if(is_skill_depends_selected(related_skill) == 0)
            {
                continue;
            }
            else
            {
                return 1;
            }
        }
    }
    return 1;
}

void Selection::count_available_but_not_used_basic_skills_in_sectors()
{
    reset_not_used_basic_skills();
    int num_of_active_skills;
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        num_of_active_skills = 3 - num_of_available_basic_skills[sector_i];
        num_of_available_but_not_used_basic_skills[sector_i] = num_of_active_skills*2 - num_of_skills_in_sector_active[sector_i] ;
    }
}

void Selection::count_selected_skills_in_sectors()
{
    reset_active_sectors();
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = sector_ptrs[sector_i];
        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
        {
            Skill** cur_circle_ptr;
            switch (circle_i)
            {
            case CIRCLE_1:  cur_circle_ptr = cur_sector_ptr->circle_1;      break;
            case CIRCLE_2:  cur_circle_ptr = cur_sector_ptr->circle_2;      break;
            case CIRCLE_3:  cur_circle_ptr = cur_sector_ptr->circle_3;      break;
            default:        cur_circle_ptr = cur_sector_ptr->base_circle;   break;
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

int Selection::find_minimum_required_base_skills(Skill *skill)
{
    int result = 1;
    if(skill->depend_type == AND)
    {
        for(Skill* related_skill : skill->depends)
        {
            if(related_skill->state == SELECTED)
            {
                continue;
            }
            result += find_minimum_required_base_skills(related_skill);
        }
        return result;
    }
    else
    {
        int min_distance = -1;
        for(Skill* related_skill : skill->depends)
        {
            if(related_skill->state == SELECTED)
            {
                min_distance = 1;
                break;
            }
            result += find_minimum_required_base_skills(related_skill);
            if(result < min_distance || min_distance == -1)
            {
                min_distance = result;
            }
        }
        return min_distance;
    }
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

void Selection::reset_hidden_skill()
{
    Skill* cur_skill;
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = sector_ptrs[sector_i];
        for(int circle_i = 0; circle_i < circle_names.size(); circle_i++)
        {
            Skill** cur_circle_ptr;
            switch (circle_i)
            {
            case CIRCLE_1:  cur_circle_ptr = cur_sector_ptr->circle_1;      break;
            case CIRCLE_2:  cur_circle_ptr = cur_sector_ptr->circle_2;      break;
            case CIRCLE_3:  cur_circle_ptr = cur_sector_ptr->circle_3;      break;
            default:        cur_circle_ptr = cur_sector_ptr->base_circle;   break;
            }
            for(int s = 0; s < 3; s++)
            {
                cur_skill = cur_circle_ptr[s];
                if(cur_skill->state == HIDDEN)
                {
                    cur_skill->state = NONE;
                    cur_skill->show();
                }
            }
        }
    }
}

void Selection::reset_not_used_basic_skills()
{
    for(int i = 0; i < sector_names.size(); i++)
    {
        num_of_available_but_not_used_basic_skills[i] = 0;
    }
}

void Selection::debug_num_of_available_basic_skills()
{
    for(int str_i = 0; str_i < 3; str_i++)
    {
        qDebug() << num_of_available_basic_skills[str_i*4 + 0] << num_of_available_basic_skills[str_i*4 + 1] << num_of_available_basic_skills[str_i*4 + 2] << num_of_available_basic_skills[str_i*4 + 3];
    }
}


////! Делает все скиллы цветными
void Selection::selection_mode_off()
{
    reset_sector_base();
    reset_active_sectors();
    reset_not_used_basic_skills();
    reset_skills_and_hide_unavailable_skills();
    emit null_scores_signal();
}
