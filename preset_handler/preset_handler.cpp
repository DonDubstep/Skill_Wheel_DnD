 #include "preset_handler.h"
#include <QDebug>

PresetHandler::PresetHandler(PageWidget** pages, QMap<QString, QVector<Skill*>> *basic_skills)
{
    for(int p = 0; p < NUM_OF_PAGES; p++)
    {
        this->page_skills_data[p] = pages[p]->get_page_skill_data();
    }
    this->basic_skills = basic_skills;
}

void PresetHandler::save_preset()
{
    qDebug() << "save pressed";
    get_active_skills();
    print_active_skills();
}

void PresetHandler::get_active_skills()
{
    Skill* cur_skill;
    QString class_name;
    int index;
    for(int page_i = 0; page_i < NUM_OF_PAGES; page_i++)
    {
        // Проверяем какие скиллы на странице активны
        for(int sector_i = 0; sector_i < sector_names.size(); sector_i++)
        {
            sector_data_t* cur_sector_ptr = page_skills_data[page_i]->sector_ptrs[sector_i];
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
                    if(cur_skill->state == SELECTED)
                    {
                        active_page_skills[page_i].append(cur_skill->index);
                    }
                }
            }
        }

        // Проверяем, какие скиллы в заголовке активны
        class_name = pages[page_i];
        for(int i = 0; i < (*basic_skills)[class_name].size(); i++)
        {
            if((*basic_skills)[class_name][i]->state == SELECTED)
            {
                index = (*basic_skills)[class_name][i]->index;
                active_basic_skills[page_i].append(index);
            }
        }
    }
}

void PresetHandler::print_active_skills()
{
    qDebug() << "page_skills:";
    for(int page_i = 0; page_i < NUM_OF_PAGES; page_i++)
    {
        qDebug() << "page" << page_i;
        QDebug dbg = qDebug();
        for(int index : active_page_skills[page_i])
        {
            dbg << index << ",";
        }
    }
    qDebug() << "basic_skills:";
    for(int page_i = 0; page_i < NUM_OF_PAGES; page_i++)
    {
        qDebug() << "page" << page_i;
        QDebug dbg = qDebug();
        for(int index : active_basic_skills[page_i])
        {
            dbg << index << ",";
        }
    }
}
