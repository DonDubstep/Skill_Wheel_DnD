#include "selection.h"
#include <QDebug>

Selection::Selection(page_skills_data_t* page_skills_data)
{
    this->page_skills_data = page_skills_data;

    reset_sector_base();
    reset_active_sectors();
    reset_not_used_basic_skills();
}

void Selection::make_dependencies()
{
    make_dependencies_regular_skills();
    make_dependencies_center_skills();
}

void Selection::make_dependencies_regular_skills()
{
    Skill* cur_skill;
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = page_skills_data->sector_ptrs[sector_i];
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

void Selection::make_dependencies_center_skills()
{
    Skill* cur_skill;
    for(int s = 0; s < 4; s++)
    {
        cur_skill = page_skills_data->center_skills[s];
        cur_skill->depend_type = parse_depend_type(cur_skill);
        cur_skill->depends = parse_depends(cur_skill);
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
    cur_skill = find_regular_skill_ptr_by_index(index);
    if(cur_skill == nullptr)
    {
        cur_skill = find_center_skill_ptr_by_index(index);
        if(cur_skill == nullptr)
        {
            qDebug() << "Не найден скилл по индексу " << index;
        }
    }
    return cur_skill;
}


Skill *Selection::find_regular_skill_ptr_by_index(int index)
{
    Skill* cur_skill;
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = page_skills_data->sector_ptrs[sector_i];
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
    return nullptr;
}

Skill *Selection::find_center_skill_ptr_by_index(int index)
{
    Skill* cur_skill;
    for(int s = 0; s < 4; s++)
    {
        cur_skill = page_skills_data->center_skills[s];
        if(cur_skill->index == index)
        {
            return cur_skill;
        }
    }
    return nullptr;
}


//! Закрашивает все скиллы серыми, если они не относятся к выбранному скиллу
void Selection::selection_mode_on(Skill* selected_skill)
{
    if(selected_skill->skill_type == SECTOR_SKILL || selected_skill->skill_type == BASE_SKILL)
    {
        select_dependencies(selected_skill);
    }
    else
    {
        select_dependencies_center_skill(selected_skill);
    }
    select_first_header_skill_from_page_selection();
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
    find_skill_in_struct(selected_skill, &sector_n, &sector, &circle_n, &skill_n);
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
            unselect_depends_base_circle_skills(skill_n, sector_n);
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
                Skill* chosen_related_skill = nullptr;
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

void Selection::select_dependencies_center_skill(Skill* selected_skill)
{
    if(selected_skill->state == SELECTED)
    {
        selected_skill->state = UNSELECTED;
        selected_skill->repaint();
    }
    else
    {
        selected_skill->state = SELECTED;
        selected_skill->repaint();
        if(selected_skill->depend_type == AND)
        {
            for(Skill* related_skill : selected_skill->depends)
            {
                if(related_skill->state != SELECTED)
                {
                    if (related_skill->skill_type == CENTER_SKILL)
                    {
                        select_dependencies_center_skill(related_skill);
                    }
                    else
                    {
                        select_dependencies(related_skill);
                    }
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
                Skill* chosen_related_skill = nullptr;
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
                if(chosen_related_skill->skill_type == CENTER_SKILL)
                {
                    select_dependencies_center_skill(chosen_related_skill);
                }
                else
                {
                    select_dependencies(chosen_related_skill);
                }
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
        sector_data_t* cur_sector_ptr = page_skills_data->sector_ptrs[sector_i];
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
        Skill* cur_skill = page_skills_data->sector_ptrs[sector_n]->base_circle[skill_n];
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
    for(int s = 0; s <= skill_n; s++)
    {
        if(num_of_available_but_not_used_basic_skills[sector_n] == 0)
            break;
        Skill* cur_skill = page_skills_data->sector_ptrs[sector_n]->base_circle[s];
        if(cur_skill->state == SELECTED)
        {
            cur_skill->state = UNSELECTED;
            cur_skill->repaint();
            num_of_available_basic_skills[sector_n]++;
            num_of_available_but_not_used_basic_skills[sector_n]--;
        }
    }
}

void Selection::find_skill_in_struct(Skill *selected_skill, int* ret_sector_n, sector_data_t **ret_sector = nullptr,  int* ret_circle_n = nullptr, int* ret_skill_i = nullptr)
{
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = page_skills_data->sector_ptrs[sector_i];
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
                    *ret_sector_n = sector_i;
                    if(ret_sector)
                        *ret_sector = cur_sector_ptr;
                    if(ret_circle_n)
                        *ret_circle_n = circle_i;
                    if(ret_skill_i)
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
    //! Проверка всех секторных скилов
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = page_skills_data->sector_ptrs[sector_i];
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
                    else
                    {
                        short is_skill_depends_selected_val = is_skill_depends_selected(cur_skill);
                        if(is_skill_depends_selected_val == 0)
                        {
                            cur_skill->state = UNSELECTED;
                            cur_skill->repaint();
                        }
                        else if(is_skill_depends_selected_val == HIDDEN)
                        {
                            cur_skill->state = HIDDEN;
                            cur_skill->hide();
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
    //! Проверка всех центральных скиллов
    for(int s = 0; s < 4; s++)
    {
        cur_skill = page_skills_data->center_skills[s];
        short is_skill_depends_selected_val = is_skill_depends_selected(cur_skill);
        if(!is_center_skill_available(cur_skill))
        {
            cur_skill->state = HIDDEN;
            cur_skill->hide();
        }
        else if(is_skill_depends_selected_val == 0)
        {
            cur_skill->state = UNSELECTED;
        }
        else if(is_skill_depends_selected_val == HIDDEN)
        {
            cur_skill->state = HIDDEN;
            cur_skill->hide();
        }
        cur_skill->repaint();
    }
}

void Selection::reset_skills_and_hide_unavailable_skills()
{
    Skill* cur_skill;
    //! Проверка всех секторных скилов
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = page_skills_data->sector_ptrs[sector_i];
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
    //! Проверка всех центральных скиллов
    for(int s = 0; s < 4; s++)
    {
        cur_skill = page_skills_data->center_skills[s];
        if(!is_center_skill_available(cur_skill))
        {
            cur_skill->state = HIDDEN;
            cur_skill->hide();
        }
        else
        {
            cur_skill->state = NONE;
            if(cur_skill->isHidden())
            {
                cur_skill->show();
            }
            cur_skill->repaint();
        }
    }
}

//! Проверка все ли зависимые скиллы уже выбраны
short Selection::is_skill_depends_selected(Skill *skill)
{
    if(skill->state != SELECTED)
        return 0;
    if(skill->state == HIDDEN)
    {
        return HIDDEN;
    }
    bool all_depends_hidden = true;
    for(int s = 0; s < skill->depends.size(); s++)
    {
        Skill* related_skill = skill->depends[s];
        if(skill->depend_type == AND)
        {
            short is_skill_depends_selected_val = is_skill_depends_selected(related_skill);
            if(is_skill_depends_selected_val == 0)
            {
                return 0;
            }
            else if(is_skill_depends_selected_val == HIDDEN)
            {
                return HIDDEN;
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
            short is_skill_depends_selected_val = is_skill_depends_selected(related_skill);
            if(is_skill_depends_selected_val == 0)
            {
                all_depends_hidden = false;
                continue;
            }
            else if(is_skill_depends_selected_val == HIDDEN)
            {
                continue;
            }
            else
            {
                return 1;
            }
        }
    }
    if(all_depends_hidden && skill->depends.size() > 0)
        return HIDDEN;
    return 1;
}

int Selection::is_center_skill_available(Skill *skill)
{
    if(skill->skill_type == CENTER_SKILL && skill->depends.size() == 0)
        return 1;
    QMap<int, int> required_base_skill_in_sector_map = find_minimum_required_base_skills_for_center_skill(skill);
    for(int sector : required_base_skill_in_sector_map.keys())
    {
        if(required_base_skill_in_sector_map[sector] > (num_of_available_basic_skills[sector] + num_of_available_but_not_used_basic_skills[sector]))
        {
            return 0;
        }
    }
    return 1;
}


QMap<int, int> Selection::find_minimum_required_base_skills_for_center_skill(Skill *skill)
{
    QMap<int, int> required_base_skill_in_sector_map;
    null_map(&required_base_skill_in_sector_map, 12);

    int sector_n = 0;
    int num_of_required_base_skills = 0;
    QMap<int, int> required_base_skill_in_sector_for_related_skill_map;
    if(skill->depend_type == AND)
    {
        for(Skill* related_skill : skill->depends)
        {
            if(related_skill->state == SELECTED)
            {
                continue;
            }
            null_map(&required_base_skill_in_sector_for_related_skill_map, 12);
            if(related_skill->skill_type == CENTER_SKILL)
            {
                required_base_skill_in_sector_for_related_skill_map = find_minimum_required_base_skills_for_center_skill(related_skill);
                for(int s = 0; s < 12; s++)
                {
                    required_base_skill_in_sector_map[s] += required_base_skill_in_sector_for_related_skill_map[s];
                }
            }
            else
            {
                num_of_required_base_skills = find_minimum_required_base_skills(related_skill);
                find_skill_in_struct(related_skill, &sector_n);
                required_base_skill_in_sector_map[sector_n] += num_of_required_base_skills;
            }
        }
    }
    else
    {
        int required_base_skill = 0;
        int min_distance = -1;
        for(Skill* related_skill : skill->depends)
        {
            if(related_skill->state == SELECTED)
            {
                null_map(&required_base_skill_in_sector_map, 12);
                return required_base_skill_in_sector_map;
            }
            if(related_skill->skill_type == CENTER_SKILL)
            {
                required_base_skill_in_sector_for_related_skill_map = find_minimum_required_base_skills_for_center_skill(related_skill);
                for(int s = 0; s < 12; s++)
                {
                    required_base_skill_in_sector_map[s] += required_base_skill_in_sector_for_related_skill_map[s];
                }
            }
            else
            {
                num_of_required_base_skills = find_minimum_required_base_skills(related_skill);
                if(num_of_required_base_skills < min_distance || min_distance == -1)
                {
                    min_distance = required_base_skill;
                    find_skill_in_struct(related_skill, &sector_n);
                }
            }
        }
        required_base_skill_in_sector_map[sector_n] += min_distance;
    }
    return required_base_skill_in_sector_map;
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
        sector_data_t* cur_sector_ptr = page_skills_data->sector_ptrs[sector_i];
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
    //! Проходимся по всем секторам
    for(int num_of_active_skills : num_of_skills_in_sector_active)
    {
        result += num_of_active_skills;
    }
    //! Проходимся по всем цетральным скиллам
    Skill* cur_skill;
    for(int s = 0; s < 4; s++)
    {
        cur_skill = page_skills_data->center_skills[s];
        if(cur_skill->state == SELECTED)
        {
            result++;
        }
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
    //! Для всех секторных скиллов
    for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
    {
        sector_data_t* cur_sector_ptr = page_skills_data->sector_ptrs[sector_i];
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
    //! Для всех центральных скиллов
    for(int s = 0; s < 4; s++)
    {
        cur_skill = page_skills_data->center_skills[s];
        if(cur_skill->state == HIDDEN)
        {
            cur_skill->state = NONE;
            cur_skill->show();
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

void Selection::select_first_header_skill_from_page_selection()
{
    if(state_of_selection_mode == 0)
    {
        emit activate_first_header_skill();
        state_of_selection_mode = 1;
    }
}

void Selection::null_map(QMap<int, int>* map, int len)
{
    for(int s = 0; s < len; s++)
    {
        (*map)[s] = 0;
    }
}

void Selection::debug_num_of_available_basic_skills()
{
    for(int str_i = 0; str_i < 3; str_i++)
    {
        qDebug() << num_of_available_basic_skills[str_i*4 + 0] << num_of_available_basic_skills[str_i*4 + 1] << num_of_available_basic_skills[str_i*4 + 2] << num_of_available_basic_skills[str_i*4 + 3];
    }
}


void Selection::activate_read_page_skills(QVector<int> *active_page_skills)
{
    // @nextfix Надо доделать, а то на невыбранных страницах выбираются нулевые скиллы, а на выбранной скрыты не те скиллы
    selection_mode_off();
    for(int s = 0; s < active_page_skills->size(); s++)
    {
        Skill* cur_skill = find_skill_ptr_by_index(active_page_skills->at(s));
        cur_skill->state = SELECTED;
    }
    reset_hidden_skill();
    count_selected_skills_in_sectors();
    count_available_but_not_used_basic_skills_in_sectors();
    hide_of_unselect_unavailable_skills();
    calculate_scores();
}

//! Делает все скиллы цветными
void Selection::selection_mode_off()
{
    reset_sector_base();
    reset_active_sectors();
    reset_not_used_basic_skills();
    reset_skills_and_hide_unavailable_skills();
    emit set_page_skills_selected_0_in_header_selection();
    emit null_scores_signal();
    state_of_selection_mode = 0;
}
